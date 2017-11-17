//
//  kern_alc.cpp
//  AppleALC
//
//  Copyright © 2016-2017 vit9696. All rights reserved.
//

#include <Headers/kern_api.hpp>
#include <Library/LegacyIOService.h>

#include <mach/vm_map.h>
#include <IOKit/IORegistryEntry.h>

#include "kern_alc.hpp"
#include "kern_resources.hpp"

// Only used in apple-driven callbacks
static AlcEnabler *callbackAlc = nullptr;
static KernelPatcher *callbackPatcher = nullptr;

bool AlcEnabler::init() {
	LiluAPI::Error error = lilu.onKextLoad(ADDPR(kextList), ADDPR(kextListSize),
	[](void *user, KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
		callbackAlc = static_cast<AlcEnabler *>(user);
		callbackPatcher = &patcher;
		callbackAlc->processKext(patcher, index, address, size);
	}, this);
	
	if (error != LiluAPI::Error::NoError) {
		SYSLOG("alc", "failed to register onKextLoad method %d", error);
		return false;
	}
	
	if (getKernelVersion() >= KernelVersion::Sierra) {
		char tmp[16];
		// Unlock custom audio engines by disabling Apple private entitlement verification
		if (PE_parse_boot_argn("-alcdhost", tmp, sizeof(tmp))) {
			error = lilu.onPatcherLoad([](void *user, KernelPatcher &patcher) {
				callbackAlc = static_cast<AlcEnabler *>(user);
				callbackPatcher = &patcher;
				callbackAlc->hookEntitlementVerification(patcher);
			}, this);
			if (error != LiluAPI::Error::NoError)
				DBGLOG("alc", "failed to register onPatcherLoad method %d", error);
		}
	}
	
	
	return true;
}

void AlcEnabler::deinit() {
	controllers.deinit();
	codecs.deinit();
}

void AlcEnabler::layoutLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context) {
	if (callbackAlc && callbackPatcher && callbackAlc->orgLayoutLoadCallback) {
		callbackAlc->updateResource(*callbackPatcher, Resource::Layout, result, resourceData, resourceDataLength);
		callbackAlc->orgLayoutLoadCallback(requestTag, result, resourceData, resourceDataLength, context);
	} else {
		SYSLOG("alc", "layout callback arrived at nowhere");
	}
}

void AlcEnabler::platformLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context) {
	if (callbackAlc && callbackPatcher && callbackAlc->orgPlatformLoadCallback) {
		callbackAlc->updateResource(*callbackPatcher, Resource::Platform, result, resourceData, resourceDataLength);
		callbackAlc->orgPlatformLoadCallback(requestTag, result, resourceData, resourceDataLength, context);
	} else {
		SYSLOG("alc", "platform callback arrived at nowhere");
	}
}

OSObject *AlcEnabler::copyClientEntitlement(task_t task, const char *entitlement) {
	if (callbackAlc && callbackAlc->orgCopyClientEntitlement) {
		auto obj = callbackAlc->orgCopyClientEntitlement(task, entitlement);
		if ((!obj || obj != kOSBooleanTrue) && !strcmp(entitlement, "com.apple.private.audio.driver-host"))
			obj = kOSBooleanTrue;
		return obj;
	}
	
	SYSLOG("alc", "copy client entitlement arrived at nowhere");
	return nullptr;
}

void AlcEnabler::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
	size_t kextIndex = 0;
	static const uint8_t logFind[] = { 0x53, 0x6F, 0x75, 0x6E, 0x64, 0x20, 0x61, 0x73, 0x73, 0x65, 0x72, 0x74, 0x69, 0x6F, 0x6E, 0x20 };
	static const uint8_t logRepl[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	
	while (kextIndex < ADDPR(kextListSize)) {
		if (ADDPR(kextList)[kextIndex].loadIndex == index)
			break;
		kextIndex++;
	}
	
	if (kextIndex == ADDPR(kextListSize))
		return;
	
	if (!(progressState & ProcessingState::ControllersLoaded)) {
		grabControllers();
		progressState |= ProcessingState::ControllersLoaded;
	} else if (!(progressState & ProcessingState::CodecsLoaded) && ADDPR(kextList)[kextIndex].user[0]) {
		if (grabCodecs())
			progressState |= ProcessingState::CodecsLoaded;
		else
			DBGLOG("alc", "failed to find a suitable codec, we have nothing to do");
	}
			   
			   
	// Continue to patch controllers
	
	if (progressState & ProcessingState::ControllersLoaded) {
		for (size_t i = 0, num = controllers.size(); i < num; i++) {
			auto &info = controllers[i]->info;
			if (!info) {
				DBGLOG("alc", "missing ControllerModInfo for %lu controller", i);
				continue;
			}
			
			applyPatches(patcher, index, info->patches, info->patchNum);
		}
    
		// patch AppleHDAController to remove redundant logs
		if (!ADDPR(debugEnabled) && !strcmp(ADDPR(kextList)[kextIndex].id, "com.apple.driver.AppleHDAController")) {
			KernelPatcher::LookupPatch hdaLogRMPatch {
				&ADDPR(kextList)[kextIndex], logFind, logRepl, sizeof(logFind), 3 // 3 occurrences inside AppleHDAController
			};
			patcher.applyLookupPatch(&hdaLogRMPatch);
			patcher.clearError();
		}
	}
	
	if (progressState & ProcessingState::CodecsLoaded) {
		for (size_t i = 0, num = codecs.size(); i < num; i++) {
			auto &info = codecs[i]->info;
			if (!info) {
				SYSLOG("alc", "missing CodecModInfo for %lu codec", i);
				continue;
			}
			
			if (info->platformNum > 0 || info->layoutNum > 0) {
				DBGLOG("alc", "will route callbacks resource loading callbacks");
				progressState |= ProcessingState::CallbacksWantRouting;
			}
			
			applyPatches(patcher, index, info->patches, info->patchNum);
		}
	}
	
	if ((progressState & ProcessingState::CallbacksWantRouting) && ADDPR(kextList)[kextIndex].user[0]) {
		auto layout = patcher.solveSymbol(index, "__ZN14AppleHDADriver18layoutLoadCallbackEjiPKvjPv");
		auto platform = patcher.solveSymbol(index, "__ZN14AppleHDADriver20platformLoadCallbackEjiPKvjPv");

		if (layout && platform) {
			DBGLOG("alc", "layout call %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X", ((uint8_t *)layout)[0], ((uint8_t *)layout)[1], ((uint8_t *)layout)[2], ((uint8_t *)layout)[3],
				   ((uint8_t *)layout)[4], ((uint8_t *)layout)[5], ((uint8_t *)layout)[6], ((uint8_t *)layout)[7], ((uint8_t *)layout)[8], ((uint8_t *)layout)[9], ((uint8_t *)layout)[10],
				   ((uint8_t *)layout)[11], ((uint8_t *)layout)[12], ((uint8_t *)layout)[13], ((uint8_t *)layout)[14], ((uint8_t *)layout)[15]);
			DBGLOG("alc", "platform call %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X %X", ((uint8_t *)platform)[0], ((uint8_t *)platform)[1], ((uint8_t *)platform)[2], ((uint8_t *)platform)[3],
				   ((uint8_t *)platform)[4], ((uint8_t *)platform)[5], ((uint8_t *)platform)[6], ((uint8_t *)platform)[7], ((uint8_t *)platform)[8], ((uint8_t *)platform)[9], ((uint8_t *)platform)[10],
				   ((uint8_t *)platform)[11], ((uint8_t *)platform)[12], ((uint8_t *)platform)[13], ((uint8_t *)platform)[14], ((uint8_t *)platform)[15]);
		}
		
		if (!layout || !platform) {
			SYSLOG("alc", "failed to find AppleHDA layout or platform callback symbols (%llX, %llX)", layout, platform);
		} else if (static_cast<void>(orgLayoutLoadCallback = reinterpret_cast<t_callback>(patcher.routeFunction(layout, reinterpret_cast<mach_vm_address_t>(layoutLoadCallback), true))),
				   patcher.getError() != KernelPatcher::Error::NoError) {
			SYSLOG("alc", "failed to hook layout callback");
		} else if (static_cast<void>(orgPlatformLoadCallback = reinterpret_cast<t_callback>(patcher.routeFunction(platform, reinterpret_cast<mach_vm_address_t>(platformLoadCallback), true))),
				   patcher.getError() != KernelPatcher::Error::NoError) {
			SYSLOG("alc", "failed to hook platform callback");
		}
    
		// patch AppleHDA to remove redundant logs
		if (!ADDPR(debugEnabled)) {
			KernelPatcher::LookupPatch hdaLogRMPatch {
				&ADDPR(kextList)[kextIndex], logFind, logRepl, sizeof(logFind), 2 // 2 occurrences inside AppleHDA
			};
			patcher.applyLookupPatch(&hdaLogRMPatch);
			patcher.clearError();
		}
	}
	
	// Ignore all the errors for other processors
	patcher.clearError();
}

void AlcEnabler::hookEntitlementVerification(KernelPatcher &patcher) {
	auto entitlement = patcher.solveSymbol(KernelPatcher::KernelID, "__ZN12IOUserClient21copyClientEntitlementEP4taskPKc");
	
	if (entitlement) {
		orgCopyClientEntitlement = reinterpret_cast<t_copyClientEntitlement>(patcher.routeFunction(entitlement, reinterpret_cast<mach_vm_address_t>(copyClientEntitlement), true));
		if (patcher.getError() != KernelPatcher::Error::NoError) {
			SYSLOG("alc", "failed to hook copy user entitlement");
		}
	}
}

void AlcEnabler::updateResource(KernelPatcher &patcher, Resource type, kern_return_t &result, const void * &resourceData, uint32_t &resourceDataLength) {
	DBGLOG("alc", "resource-request arrived %s", type == Resource::Platform ? "platform" : "layout");
	
	for (size_t i = 0, s = codecs.size(); i < s; i++) {
		DBGLOG("alc", "checking codec %X:%X:%X", codecs[i]->vendor, codecs[i]->codec, codecs[i]->revision);
		
		auto info = codecs[i]->info;
		if (!info) {
			SYSLOG("alc", "missing CodecModInfo for %lu codec at resource updating", i);
			continue;
		}
		
		if ((type == Resource::Platform && info->platforms) || (type == Resource::Layout && info->layouts)) {
			size_t num = type == Resource::Platform ? info->platformNum : info->layoutNum;
			DBGLOG("alc", "selecting from %lu files", num);
			for (size_t f = 0; f < num; f++) {
				auto &fi = (type == Resource::Platform ? info->platforms : info->layouts)[f];
				DBGLOG("alc", "comparing %lu layout %X/%X", f, fi.layout, controllers[codecs[i]->controller]->layout);
				if (controllers[codecs[i]->controller]->layout == fi.layout && patcher.compatibleKernel(fi.minKernel, fi.maxKernel)) {
					DBGLOG("alc", "found %s at %lu index", type == Resource::Platform ? "platform" : "layout", f);
					resourceData = fi.data;
					resourceDataLength = fi.dataLength;
					result = kOSReturnSuccess;
					break;
				}
			}
		}
	}
}

void AlcEnabler::grabControllers() {
	computerModel = WIOKit::getComputerModel();
	
	bool found {false};
	
	for (size_t lookup = 0; lookup < ADDPR(codecLookupSize); lookup++) {
		auto sect = WIOKit::findEntryByPrefix("/AppleACPIPlatformExpert", "PCI", gIOServicePlane);
		
		for (size_t i = 0; sect && i <= ADDPR(codecLookup)[lookup].controllerNum; i++) {
			sect = WIOKit::findEntryByPrefix(sect, ADDPR(codecLookup)[lookup].tree[i], gIOServicePlane);
			
			if (sect && i == ADDPR(codecLookup)[lookup].controllerNum) {
				// Nice, we found some controller, add it
				uint32_t ven {0}, dev {0}, rev {0}, platform {ControllerModInfo::PlatformAny}, lid {0};
				
				if (!WIOKit::getOSDataValue(sect, "vendor-id", ven) ||
					!WIOKit::getOSDataValue(sect, "device-id", dev) ||
					!WIOKit::getOSDataValue(sect, "revision-id", rev)) {
					SYSLOG("alc", "found an incorrect controller at %s", ADDPR(codecLookup)[lookup].tree[i]);
					break;
				}
				
				if (ADDPR(codecLookup)[lookup].detect && !WIOKit::getOSDataValue(sect, "layout-id", lid)) {
					SYSLOG("alc", "layout-id was not provided by controller at %s", ADDPR(codecLookup)[lookup].tree[i]);
					break;
				}
				
				if (WIOKit::getOSDataValue(sect, "AAPL,ig-platform-id", platform)) {
					DBGLOG("alc", "AAPL,ig-platform-id %X was found in controller at %s", platform, ADDPR(codecLookup)[lookup].tree[i]);
				} else if (WIOKit::getOSDataValue(sect, "AAPL,snb-platform-id", platform)) {
					DBGLOG("alc", "AAPL,snb-platform-id %X was found in controller at %s", platform, ADDPR(codecLookup)[lookup].tree[i]);
				}
				
				auto controller = ControllerInfo::create(ven, dev, rev, platform, lid, ADDPR(codecLookup)[lookup].detect);
				if (controller) {
					if (controllers.push_back(controller)) {
						controller->lookup = &ADDPR(codecLookup)[lookup];
						found = true;
					} else {
						SYSLOG("alc", "failed to store controller info for %X:%X:%X", ven, dev, rev);
						ControllerInfo::deleter(controller);
					}
				} else {
					SYSLOG("alc", "failed to create controller info for %X:%X:%X", ven, dev, rev);
				}
			}
		}
	}
	
	if (found) {
		DBGLOG("alc", "found %lu audio controllers", controllers.size());
		validateControllers();
	}
}

bool AlcEnabler::grabCodecs() {
	for (currentController = 0; currentController < controllers.size(); currentController++) {
		auto ctlr = controllers[currentController];
		
		// Digital controllers normally have no detectible codecs
		if (!ctlr->detect)
			continue;

		auto sect = WIOKit::findEntryByPrefix("/AppleACPIPlatformExpert", "PCI", gIOServicePlane);

		for (size_t i = 0; sect && i < ctlr->lookup->treeSize; i++) {
			bool last = i+1 == ctlr->lookup->treeSize;
			sect = WIOKit::findEntryByPrefix(sect, ctlr->lookup->tree[i], gIOServicePlane,
											 last ? [](void *user, IORegistryEntry *e) {
	
				auto alc = static_cast<AlcEnabler *>(user);
												 
				auto ven = e->getProperty("IOHDACodecVendorID");
				auto rev = e->getProperty("IOHDACodecRevisionID");
				
				
				if (!ven || !rev) {
					DBGLOG("alc", "codec entry misses properties, skipping");
					return false;
				}
				
				auto venNum = OSDynamicCast(OSNumber, ven);
				auto revNum = OSDynamicCast(OSNumber, rev);
				
				if (!venNum || !revNum) {
					SYSLOG("alc", "codec entry contains invalid properties, skipping");
					return true;
				}
				
				auto ci = AlcEnabler::CodecInfo::create(alc->currentController,
														venNum->unsigned64BitValue(),
														revNum->unsigned32BitValue());
				if (ci) {
					if (!alc->codecs.push_back(ci)) {
						SYSLOG("alc", "failed to store codec info for %X:%X:%X", ci->vendor, ci->codec, ci->revision);
						AlcEnabler::CodecInfo::deleter(ci);
					}
				} else {
					SYSLOG("alc", "failed to create codec info for %X %X:%X", ci->vendor, ci->codec, ci->revision);
				}
				
				return true;
			
			} : nullptr, last, this);
		}
	}

	return validateCodecs();
}

void AlcEnabler::validateControllers() {
	for (size_t i = 0, num = controllers.size(); i < num; i++) {
		DBGLOG("alc", "validating %lu controller %X:%X:%X", i, controllers[i]->vendor, controllers[i]->device, controllers[i]->revision);
		for (size_t mod = 0; mod < ADDPR(controllerModSize); mod++) {
			DBGLOG("alc", "comparing to %lu mod %X:%X", mod, ADDPR(controllerMod)[mod].vendor, ADDPR(controllerMod)[mod].device);
			if (controllers[i]->vendor == ADDPR(controllerMod)[mod].vendor &&
				controllers[i]->device == ADDPR(controllerMod)[mod].device) {
				
				// Check revision if present
				size_t rev {0};
				while (rev < ADDPR(controllerMod)[mod].revisionNum &&
					   ADDPR(controllerMod)[mod].revisions[rev] != controllers[i]->revision)
					rev++;
				
				// Check AAPL,ig-platform-id if present
				if (ADDPR(controllerMod)[mod].platform != ControllerModInfo::PlatformAny &&
					ADDPR(controllerMod)[mod].platform != controllers[i]->platform) {
					DBGLOG("alc", "not matching platform was found %X vs %X", ADDPR(controllerMod)[mod].platform, controllers[i]->platform);
					continue;
				}
				
				// Check if computer model is suitable
				if (!(computerModel & ADDPR(controllerMod)[mod].computerModel)) {
					DBGLOG("alc", "unsuitable computer model was found %X vs %X", ADDPR(controllerMod)[mod].computerModel, computerModel);
					continue;
				}
			
				if (rev != ADDPR(controllerMod)[mod].revisionNum ||
					ADDPR(controllerMod)[mod].revisionNum == 0) {
					DBGLOG("alc", "found mod for %lu controller", i);
					controllers[i]->info = &ADDPR(controllerMod)[mod];
					break;
				}
			}
		}
	}
}

bool AlcEnabler::validateCodecs() {
	size_t i = 0;
	
	while (i < codecs.size()) {
		bool suitable {false};
		
		// Check vendor
		size_t vIdx {0};
		while (vIdx < ADDPR(vendorModSize) && ADDPR(vendorMod)[vIdx].vendor != codecs[i]->vendor)
			vIdx++;
		
		if (vIdx != ADDPR(vendorModSize)) {
			// Check codec
			size_t cIdx {0};
			while (cIdx < ADDPR(vendorMod)[vIdx].codecsNum &&
				   ADDPR(vendorMod)[vIdx].codecs[cIdx].codec != codecs[i]->codec)
				cIdx++;
			
			if (cIdx != ADDPR(vendorMod)[vIdx].codecsNum) {
				// Check revision if present
				size_t rIdx {0};
				while (rIdx < ADDPR(vendorMod)[vIdx].codecs[cIdx].revisionNum &&
					   ADDPR(vendorMod)[vIdx].codecs[cIdx].revisions[rIdx] != codecs[i]->revision)
					rIdx++;
				
				if (rIdx != ADDPR(vendorMod)[vIdx].codecs[cIdx].revisionNum ||
					ADDPR(vendorMod)[vIdx].codecs[cIdx].revisionNum == 0) {
					codecs[i]->info = &ADDPR(vendorMod)[vIdx].codecs[cIdx];
					suitable = true;
					
				}
				
				DBGLOG("alc", "found %s %s %s codec revision 0x%X",
					   suitable ? "supported" : "unsupported", ADDPR(vendorMod)[vIdx].name,
					   ADDPR(vendorMod)[vIdx].codecs[cIdx].name, codecs[i]->revision);
			} else {
				DBGLOG("alc", "found unsupported %s codec 0x%X revision 0x%X", ADDPR(vendorMod)[vIdx].name,
					   codecs[i]->codec, codecs[i]->revision);
			}
		} else {
			DBGLOG("alc", "found unsupported codec vendor 0x%X", codecs[i]->vendor);
		}
		
		if (suitable)
			i++;
		else
			codecs.erase(i);
	}

	return codecs.size() > 0;
}

void AlcEnabler::applyPatches(KernelPatcher &patcher, size_t index, const KextPatch *patches, size_t patchNum) {
	DBGLOG("alc", "applying patches for %lu kext", index);
	for (size_t p = 0; p < patchNum; p++) {
		auto &patch = patches[p];
		if (patch.patch.kext->loadIndex == index) {
			if (patcher.compatibleKernel(patch.minKernel, patch.maxKernel)) {
				DBGLOG("alc", "applying %lu patch for %lu kext", p, index);
				patcher.applyLookupPatch(&patch.patch);
				// Do not really care for the errors for now
				patcher.clearError();
			}
		}
	}
}
