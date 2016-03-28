//
//  kern_alc.cpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#include "kern_alc.hpp"
#include "kern_iokit.hpp"
#include "kern_resources.hpp"

#include <mach/vm_map.h>
#include <IOKit/IORegistryEntry.h>
#include <IOKit/IOService.h>

//TODO: get rid of this
static AlcEnabler *that {nullptr};

bool AlcEnabler::init() {
	progressState = ProcessingState::NotReady;

	patcher.init();
	
	if (patcher.getError() != KernelPatcher::Error::NoError) {
		DBGLOG("alc @ failed to initialise kernel patcher");
		patcher.clearError();
		return false;
	}
	
	return loadKexts();
}

void AlcEnabler::deinit() {
	patcher.deinit();
	controllers.deinit();
	codecs.deinit();
}

void AlcEnabler::layoutLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context) {
	if (that && that->orgLayoutLoadCallback) {
		that->updateResource(Resource::Layout, resourceData, resourceDataLength);
		that->orgLayoutLoadCallback(requestTag, result, resourceData, resourceDataLength, context);
	} else {
		SYSLOG("alc @ layout callback arrived at nowhere");
	}
}

void AlcEnabler::platformLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context) {
	if (that && that->orgPlatformLoadCallback) {
		that->updateResource(Resource::Platform, resourceData, resourceDataLength);
		that->orgPlatformLoadCallback(requestTag, result, resourceData, resourceDataLength, context);
	} else {
		SYSLOG("alc @ platform callback arrived at nowhere");
	}
}

bool AlcEnabler::loadKexts() {
	if (that) return true;
	
	for (size_t i = 0; i < kextListSize; i++) {
		patcher.loadKinfo(&kextList[i]);
		if (patcher.getError() != KernelPatcher::Error::NoError) {
			SYSLOG("alc @ failed to load %s kext file", kextList[i].id);
			patcher.clearError();
			return false;
		}
		
		patcher.setupKextListening();
		
		if (patcher.getError() != KernelPatcher::Error::NoError) {
			SYSLOG("alc @ failed to setup kext hooking");
			patcher.clearError();
			return false;
		}
		
		auto handler = KernelPatcher::KextHandler::create(kextList[i].id, kextList[i].loadIndex,
		[](KernelPatcher::KextHandler *h) {
			if (h && that) {
				that->processKext(h->index, h->address, h->size);
			} else {
				SYSLOG("alc @ notification callback arrived at nowhere");
			}
		});
		
		if (!handler) {
			SYSLOG("alc @ failed to allocate KextHandler");
			return false;
		}
		
		patcher.waitOnKext(handler);
		
		if (patcher.getError() != KernelPatcher::Error::NoError) {
			SYSLOG("alc @ failed to wait on kext");
			patcher.clearError();
			KernelPatcher::KextHandler::deleter(handler);
			return false;
		}
	
	}
	
	that = this;
	return true;
}

void AlcEnabler::processKext(size_t index, mach_vm_address_t address, size_t size) {
	patcher.updateRunningInfo(index, address, size);
	
	if (patcher.getError() == KernelPatcher::Error::NoError) {
		if (!(progressState & ProcessingState::ControllersLoaded)) {
			grabControllers();
			progressState |= ProcessingState::ControllersLoaded;
		} else if (!(progressState & ProcessingState::CodecsLoaded)) {
			for (size_t i = 0; i < kextListSize; i++) {
				if (kextList[i].loadIndex == index) {
					if (kextList[i].detectCodecs && grabCodecs()) {
						progressState |= ProcessingState::CodecsLoaded;
						break;
					}
					DBGLOG("alc @ failed to find a suitable codec, we have nothing to do");
					// Continue to patch controllers
				}
			}
		}
	
		if (progressState & ProcessingState::ControllersLoaded) {
			for (size_t i = 0, num = controllers.size(); i < num; i++) {
				auto &info = controllers[i]->info;
				if (!info) {
					DBGLOG("alc @ missing ControllerModInfo for %zu controller", i);
					continue;
				}
				
				applyPatches(index, info->patches, info->patchNum);
			}
		}
		
		if (progressState & ProcessingState::CodecsLoaded) {
			for (size_t i = 0, num = codecs.size(); i < num; i++) {
				auto &info = codecs[i]->info;
				if (!info) {
					SYSLOG("alc @ missing CodecModInfo for %zu codec", i);
					continue;
				}
				
				if (info->platforms > 0 && info->layoutNum > 0) {
					DBGLOG("alc @ will route callbacks resource loading callbacks");
					progressState |= ProcessingState::CallbacksWantRouting;
				}
				
				applyPatches(index, info->patches, info->patchNum);
			}
		}
		
		if ((progressState & ProcessingState::CallbacksWantRouting) && !(progressState & ProcessingState::CallbacksRouted)) {
			auto layout = patcher.solveSymbol(index, "__ZN14AppleHDADriver18layoutLoadCallbackEjiPKvjPv");
			auto platform = patcher.solveSymbol(index, "__ZN14AppleHDADriver20platformLoadCallbackEjiPKvjPv");

			if (!layout || !platform) {
				SYSLOG("alc @ failed to find AppleHDA layout or platform callback symbols (%llX, %llX)", layout, platform);
			} else if (orgLayoutLoadCallback = reinterpret_cast<t_callback>(patcher.routeFunction(layout, reinterpret_cast<mach_vm_address_t>(layoutLoadCallback), true)),
					   patcher.getError() != KernelPatcher::Error::NoError) {
				SYSLOG("alc @ failed to hook layout callback");
			} else if (orgPlatformLoadCallback = reinterpret_cast<t_callback>(patcher.routeFunction(platform, reinterpret_cast<mach_vm_address_t>(platformLoadCallback), true)),
					   patcher.getError() != KernelPatcher::Error::NoError) {
				SYSLOG("alc @ failed to hook platform callback");
			} else {
				progressState |= ProcessingState::CallbacksRouted;
			}
		}
	} else {
		SYSLOG("alc @ failed to update kext running info");
	}
	
	// Ignore all the errors for other processors
	patcher.clearError();
}

void AlcEnabler::updateResource(Resource type, const void * &resourceData, uint32_t &resourceDataLength) {
	DBGLOG("alc @ resource-request arrived %s", type == Resource::Platform ? "paltform" : "layout");
	
	for (size_t i = 0, s = codecs.size(); i < s; i++) {
		DBGLOG("alc @ checking codec %X:%X:%X", codecs[i]->vendor, codecs[i]->codec, codecs[i]->revision);
		
		auto info = codecs[i]->info;
		if (!info) {
			SYSLOG("alc @ missing CodecModInfo for %zu codec at resource updating", i);
			continue;
		}
		
		if ((type == Resource::Platform && info->platforms) || (type == Resource::Layout && info->layouts)) {
			size_t num = type == Resource::Platform ? info->platformNum : info->layoutNum;
			DBGLOG("alc @ selecting from %zu files", num);
			for (size_t f = 0; f < num; f++) {
				auto &fi = (type == Resource::Platform ? info->platforms : info->layouts)[f];
				DBGLOG("alc @ comparing %zu layout %X/%X", f, fi.layout, controllers[codecs[i]->controller]->layout);
				if (controllers[codecs[i]->controller]->layout == fi.layout && patcher.compatibleKernel(fi.minKernel, fi.maxKernel)) {
					DBGLOG("Found %s at %zu index", type == Resource::Platform ? "platform" : "layout", f);
					resourceData = fi.data;
					resourceDataLength = fi.dataLength;
					break;
				}
			}
		}
	}
}

void AlcEnabler::grabControllers() {
	if (!that) {
		SYSLOG("alc @ you should call grabCodecs right before AppleHDAController loading");
		return;
	}
	
	computerModel = IOUtil::getComputerModel();
	
	bool found {false};
	
	for (size_t lookup = 0; lookup < codecLookupSize; lookup++) {
		auto sect = IOUtil::findEntryByPrefix("/AppleACPIPlatformExpert", "PCI", gIOServicePlane);
		
		for (size_t i = 0; sect && i <= codecLookup[lookup].controllerNum; i++) {
			sect = IOUtil::findEntryByPrefix(sect, codecLookup[lookup].tree[i], gIOServicePlane);
			
			if (sect && i == codecLookup[lookup].controllerNum) {
				// Nice, we found some controller, add it
				uint32_t ven {0}, dev {0}, rev {0}, platform {ControllerModInfo::PlatformAny}, lid {0};
				
				if (!IOUtil::getOSDataValue(sect, "vendor-id", ven) ||
					!IOUtil::getOSDataValue(sect, "device-id", dev) ||
					!IOUtil::getOSDataValue(sect, "revision-id", rev)) {
					SYSLOG("alc @ found an incorrect controller at %s", codecLookup[lookup].tree[i]);
					break;
				}
				
				if (codecLookup[lookup].detect && !IOUtil::getOSDataValue(sect, "layout-id", lid)) {
					SYSLOG("alc @ layout-id was not provided by controller at %s", codecLookup[lookup].tree[i]);
					break;
				}
				
				if (IOUtil::getOSDataValue(sect, "AAPL,ig-platform-id", platform)) {
					DBGLOG("alc @ AAPL,ig-platform-id %X was found in controller at %s", platform, codecLookup[lookup].tree[i]);
				}
				
				auto controller = ControllerInfo::create(ven, dev, rev, platform, lid, codecLookup[lookup].detect);
				if (controller) {
					if (controllers.push_back(controller)) {
						controller->lookup = &codecLookup[lookup];
						found = true;
					} else {
						SYSLOG("alc @ failed to store controller info for %X:%X:%X", ven, dev, rev);
						ControllerInfo::deleter(controller);
					}
				} else {
					SYSLOG("alc @ failed to create controller info for %X:%X:%X", ven, dev, rev);
				}
			}
		}
	}
	
	if (found) {
		DBGLOG("alc @ found %zu audio controllers", controllers.size());
		validateControllers();
	}
}

bool AlcEnabler::grabCodecs() {
	if (!that) {
		SYSLOG("alc @ you should call grabCodecs right before AppleHDA loading");
		return false;
	}
	
	for (currentController = 0; currentController < controllers.size(); currentController++) {
		auto ctlr = controllers[currentController];
		
		// Digital controllers normally have no detectible codecs
		if (!ctlr->detect)
			continue;

		auto sect = IOUtil::findEntryByPrefix("/AppleACPIPlatformExpert", "PCI", gIOServicePlane);

		for (size_t i = 0; sect && i < ctlr->lookup->treeSize; i++) {
			bool last = i+1 == ctlr->lookup->treeSize;
			sect = IOUtil::findEntryByPrefix(sect, ctlr->lookup->tree[i], gIOServicePlane,
											 last ? [](IORegistryEntry *e) {
				
				auto ven = e->getProperty("IOHDACodecVendorID");
				auto rev = e->getProperty("IOHDACodecRevisionID");
				
				if (!ven || !rev) {
					DBGLOG("alc @ codec entry misses properties, skipping");
					return false;
				}
				
				auto venNum = OSDynamicCast(OSNumber, ven);
				auto revNum = OSDynamicCast(OSNumber, rev);
				
				if (!venNum || !revNum) {
					SYSLOG("alc @ codec entry contains invalid properties, skipping");
					return true;
				}
				
				auto ci = AlcEnabler::CodecInfo::create(that->currentController, venNum->unsigned64BitValue(),
														revNum->unsigned32BitValue());
				if (ci) {
					if (!that->codecs.push_back(ci)) {
						SYSLOG("alc @ failed to store codec info for %X:%X:%X", ci->vendor, ci->codec, ci->revision);
						AlcEnabler::CodecInfo::deleter(ci);
					}
				} else {
					SYSLOG("alc @ failed to create codec info for %X %X:%X", ci->vendor, ci->codec, ci->revision);
				}
				
				return true;
			
			} : nullptr, last);
		}
	}

	return validateCodecs();
}

void AlcEnabler::validateControllers() {
	for (size_t i = 0, num = controllers.size(); i < num; i++) {
		DBGLOG("alc @ validating %zu controller %X:%X:%X", i, controllers[i]->vendor, controllers[i]->device, controllers[i]->revision);
		for (size_t mod = 0; mod < controllerModSize; mod++) {
			DBGLOG("alc @ comparing to %zu mod %X:%X", mod, controllerMod[mod].vendor, controllerMod[mod].device);
			if (controllers[i]->vendor == controllerMod[mod].vendor &&
				controllers[i]->device == controllerMod[mod].device) {
				
				// Check revision if present
				size_t rev {0};
				while (rev < controllerMod[mod].revisionNum &&
					   controllerMod[mod].revisions[rev] != controllers[i]->revision)
					rev++;
				
				// Check AAPL,ig-platform-id if present
				if (controllerMod[mod].platform != ControllerModInfo::PlatformAny &&
					controllerMod[mod].platform != controllers[i]->platform) {
					DBGLOG("alc @ not matching platform was found %X vs %X", controllerMod[mod].platform, controllers[i]->platform);
					continue;
				}
				
				// Check if computer model is suitable
				if (!(computerModel & controllerMod[mod].computerModel)) {
					DBGLOG("alc @ unsuitable computer model was found %X vs %X", controllerMod[mod].computerModel, computerModel);
					continue;
				}
			
				if (rev != controllerMod[mod].revisionNum ||
					controllerMod[mod].revisionNum == 0) {
					DBGLOG("alc @ found mod for %zu controller", i);
					controllers[i]->info = &controllerMod[mod];
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
		while (vIdx < vendorModSize && vendorMod[vIdx].vendor != codecs[i]->vendor)
			vIdx++;
		
		if (vIdx != vendorModSize) {
			// Check codec
			size_t cIdx {0};
			while (cIdx < vendorMod[vIdx].codecsNum &&
				   vendorMod[vIdx].codecs[cIdx].codec != codecs[i]->codec)
				cIdx++;
			
			if (cIdx != vendorMod[vIdx].codecsNum) {
				// Check revision if present
				size_t rIdx {0};
				while (rIdx < vendorMod[vIdx].codecs[cIdx].revisionNum &&
					   vendorMod[vIdx].codecs[cIdx].revisions[rIdx] != codecs[i]->revision)
					rIdx++;
				
				if (rIdx != vendorMod[vIdx].codecs[cIdx].revisionNum ||
					vendorMod[vIdx].codecs[cIdx].revisionNum == 0) {
					codecs[i]->info = &vendorMod[vIdx].codecs[cIdx];
					suitable = true;
					
				}
				
				DBGLOG("alc @ found %s %s %s codec revision 0x%X",
					   suitable ? "supported" : "unsupported", vendorMod[vIdx].name,
					   vendorMod[vIdx].codecs[cIdx].name, codecs[i]->revision);
			} else {
				DBGLOG("alc @ found unsupported %s codec 0x%X revision 0x%X", vendorMod[vIdx].name,
					   codecs[i]->codec, codecs[i]->revision);
			}
		} else {
			DBGLOG("alc @ found unsupported codec vendor 0x%X", codecs[i]->vendor);
		}
		
		if (suitable)
			i++;
		else
			codecs.erase(i);
	}

	return codecs.size() > 0;
}

void AlcEnabler::applyPatches(size_t index, const KextPatch *patches, size_t patchNum) {
	DBGLOG("alc @ applying patches for %zu kext", index);
	for (size_t p = 0; p < patchNum; p++) {
		auto &patch = patches[p];
		if (patch.patch.kext->loadIndex == index) {
			if (patcher.compatibleKernel(patch.minKernel, patch.maxKernel)) {
				DBGLOG("alc @ applying %zu patch for %zu kext", p, index);
				patcher.applyLookupPatch(&patch.patch);
				// Do not really care for the errors for now
				patcher.clearError();
			}
		}
	}
}