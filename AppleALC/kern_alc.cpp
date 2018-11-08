//
//  kern_alc.cpp
//  AppleALC
//
//  Copyright © 2016-2017 vit9696. All rights reserved.
//

#include <Headers/kern_api.hpp>
#include <Headers/kern_devinfo.hpp>
#include <Headers/plugin_start.hpp>
#include <Library/LegacyIOService.h>
#include <mach/vm_map.h>

#include "kern_alc.hpp"
#include "kern_resources.hpp"

// Only used in apple-driven callbacks
static AlcEnabler *callbackAlc;

void AlcEnabler::init() {
	callbackAlc = this;

	lilu.onPatcherLoadForce(
	[](void *user, KernelPatcher &pathcer) {
		static_cast<AlcEnabler *>(user)->updateProperties();
	}, this);

	if (getKernelVersion() < KernelVersion::Mojave)
		ADDPR(kextList)[KextIdAppleGFXHDA].switchOff();

	lilu.onKextLoadForce(ADDPR(kextList), ADDPR(kextListSize),
	[](void *user, KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
		static_cast<AlcEnabler *>(user)->processKext(patcher, index, address, size);
	}, this);

	if (getKernelVersion() >= KernelVersion::Sierra) {
		// Unlock custom audio engines by disabling Apple private entitlement verification
		if (checkKernelArgument("-alcdhost")) {
			lilu.onEntitlementRequestForce([](void *user, task_t task, const char *entitlement, OSObject *&original) {
				static_cast<AlcEnabler *>(user)->handleAudioClientEntitlement(task, entitlement, original);
			}, this);
		}
	}
}

void AlcEnabler::deinit() {
	controllers.deinit();
	codecs.deinit();
}

void AlcEnabler::updateProperties() {
	auto devInfo = DeviceInfo::create();
	if (devInfo) {
		// Assume that IGPU with connections means built-in digital audio.
		bool hasBuiltinDigitalAudio = !devInfo->reportedFramebufferIsConnectorLess && devInfo->videoBuiltin;

		// Firstly, update Haswell or Broadwell HDAU device for built-in digital audio.
		if (devInfo->audioBuiltinDigital) {
			if (hasBuiltinDigitalAudio) {
				// This is a normal HDAU device for an IGPU with connectors.
				updateDeviceProperties(devInfo->audioBuiltinDigital, devInfo, "onboard-1", false);
				uint32_t dev = 0, rev = 0;
				if (WIOKit::getOSDataValue(devInfo->audioBuiltinDigital, "device-id", dev) &&
					WIOKit::getOSDataValue(devInfo->audioBuiltinDigital, "revision-id", rev))
					insertController(WIOKit::VendorID::Intel, dev, rev, nullptr != devInfo->audioBuiltinDigital->getProperty("no-controller-patch"), devInfo->reportedFramebufferId);
			} else {
				// Terminate built-in HDAU audio, as we are using no connectors!
				auto hda = OSDynamicCast(IOService, devInfo->audioBuiltinDigital);
				auto pci = OSDynamicCast(IOService, devInfo->audioBuiltinDigital->getParentEntry(gIOServicePlane));
				if (hda && pci) {
					if (hda->requestTerminate(pci, 0) && hda->terminate())
						hda->stop(pci);
					else
						SYSLOG("alc", "failed to terminate built-in digital audio");
				} else {
					SYSLOG("alc", "incompatible built-in hdau discovered");
				}
			}
		}

		// Secondly, update HDEF device and make it support digital audio
		if (devInfo->audioBuiltinAnalog) {
			const char *hdaGfx = nullptr;
			if (hasBuiltinDigitalAudio && !devInfo->audioBuiltinDigital)
				hdaGfx = "onboard-1";
			updateDeviceProperties(devInfo->audioBuiltinAnalog, devInfo, hdaGfx, true);
		}

		// Thirdly, update IGPU device in case we have digital audio
		if (hasBuiltinDigitalAudio) {
			devInfo->videoBuiltin->setProperty("hda-gfx", const_cast<char *>("onboard-1"), sizeof("onboard-1"));
			if (!devInfo->audioBuiltinDigital) {
				uint32_t dev = 0, rev = 0;
				if (WIOKit::getOSDataValue(devInfo->videoBuiltin, "device-id", dev) &&
					WIOKit::getOSDataValue(devInfo->videoBuiltin, "revision-id", rev))
					insertController(WIOKit::VendorID::Intel, dev, rev, nullptr != devInfo->videoBuiltin->getProperty("no-controller-patch"), devInfo->reportedFramebufferId);
			}
		}

		uint32_t hdaGfxCounter = hasBuiltinDigitalAudio ? 2 : 1;

		// Fourthly, update all the GPU devices if any
		for (size_t gpu = 0; gpu < devInfo->videoExternal.size(); gpu++) {
			auto hdaSevice = devInfo->videoExternal[gpu].audio;
			auto gpuService = devInfo->videoExternal[gpu].video;

			if (!hdaSevice)
				continue;

			uint32_t ven = devInfo->videoExternal[gpu].vendor;
			uint32_t dev = 0, rev = 0;
			if (WIOKit::getOSDataValue(hdaSevice, "device-id", dev) &&
				WIOKit::getOSDataValue(hdaSevice, "revision-id", rev)) {
				// Register the controller
				insertController(ven, dev, rev, nullptr != hdaSevice->getProperty("no-controller-patch"));
				// Disable the id in the list if any
				if (ven == WIOKit::VendorID::NVIDIA) {
					uint32_t device = (dev << 16) | WIOKit::VendorID::NVIDIA;
					for (size_t i = 0; i < MaxNvidiaDeviceIds; i++)
						if (nvidiaDeviceIdList[i] == device)
							nvidiaDeviceIdUsage[i] = true;
				}
			}

			// Refresh the main properties including hda-gfx.
			char hdaGfx[16];
			snprintf(hdaGfx, sizeof(hdaGfx), "onboard-%u", hdaGfxCounter++);
			updateDeviceProperties(hdaSevice, devInfo, hdaGfx, false);
			gpuService->setProperty("hda-gfx", hdaGfx, static_cast<uint32_t>(strlen(hdaGfx)+1));

			// Refresh connector types on NVIDIA, since they are required for HDMI audio to function.
			// Abort if preexisting connector-types or no-audio-fixconn property is found.
			if (ven == WIOKit::VendorID::NVIDIA && !gpuService->getProperty("no-audio-fixconn")) {
				uint8_t builtBytes[] { 0x00, 0x08, 0x00, 0x00 };
				char connector_type[] { "@0,connector-type" };
				for (size_t i = 0; i < MaxConnectorCount; i++) {
					connector_type[1] = '0' + i;
					if (!gpuService->getProperty(connector_type)) {
						DBGLOG("alc", "fixing %s in gpu", connector_type);
						gpuService->setProperty(connector_type, builtBytes, sizeof(builtBytes));
					} else {
						DBGLOG("alc", "found existing %s in gpu", connector_type);
						break;
					}
				}
			}
		}

		DeviceInfo::deleter(devInfo);
	}
}

void AlcEnabler::updateDeviceProperties(IORegistryEntry *hdaService, DeviceInfo *info, const char *hdaGfx, bool isAnalog) {
	auto hdaPlaneName = hdaService->getName();

	// AppleHDAController only recognises HDEF and HDAU.
	if (isAnalog && (!hdaPlaneName || strcmp(hdaPlaneName, "HDEF") != 0)) {
		DBGLOG("alc", "fixing audio plane name to HDEF");
		WIOKit::renameDevice(hdaService, "HDEF");
	} else if (!isAnalog && (!hdaPlaneName || strcmp(hdaPlaneName, "HDAU") != 0)) {
		DBGLOG("alc", "fixing audio plane name to HDAU");
		WIOKit::renameDevice(hdaService, "HDAU");
	}

	if (isAnalog) {
		// Refresh our own layout-id named alc-layout-id as follows:
		// alcid=X has highest priority and overrides any other value.
		// alc-layout-id has normal priority and is expected to be used.
		// layout-id will be used if both alcid and alc-layout-id are not set on non-Apple platforms.
		uint32_t layout = 0;
		if (PE_parse_boot_argn("alcid", &layout, sizeof(layout))) {
			DBGLOG("alc", "found alc-layout-id override %u", layout);
			hdaService->setProperty("alc-layout-id", &layout, sizeof(layout));
		} else {
			uint32_t alcId;
			if (WIOKit::getOSDataValue(hdaService, "alc-layout-id", alcId)) {
				DBGLOG("alc", "found normal alc-layout-id %u", alcId);
			} else if (info->firmwareVendor != DeviceInfo::FirmwareVendor::Apple &&
					   WIOKit::getOSDataValue(hdaService, "layout-id", alcId)) {
				DBGLOG("audio", "found legacy alc-layout-id (from layout-id) %u", alcId);
				hdaService->setProperty("alc-layout-id", &alcId, sizeof(alcId));
			}
		}

		// These seem to fix AppleHDA warnings, perhaps research them later.
		// They are probably related to the current volume of the boot bell sound.
		if (!hdaService->getProperty("MaximumBootBeepVolume")) {
			DBGLOG("alc", "fixing MaximumBootBeepVolume in hdef");
			uint8_t bootBeepBytes[] { 0xEE };
			hdaService->setProperty("MaximumBootBeepVolume", bootBeepBytes, sizeof(bootBeepBytes));
		}

		if (!hdaService->getProperty("MaximumBootBeepVolumeAlt")) {
			DBGLOG("alc", "fixing MaximumBootBeepVolumeAlt in hdef");
			uint8_t bootBeepBytes[] { 0xEE };
			hdaService->setProperty("MaximumBootBeepVolumeAlt", bootBeepBytes, sizeof(bootBeepBytes));
		}

		if (!hdaService->getProperty("PinConfigurations")) {
			DBGLOG("alc", "fixing PinConfigurations in hdef");
			uint8_t pinBytes[] { 0x00 };
			hdaService->setProperty("PinConfigurations", pinBytes, sizeof(pinBytes));
		}
	}

	// For every client only set layout-id itself.
	if (info->firmwareVendor != DeviceInfo::FirmwareVendor::Apple)
		hdaService->setProperty("layout-id", &info->reportedLayoutId, sizeof(info->reportedLayoutId));

	// Pass onboard-X if requested.
	if (hdaGfx)
		hdaService->setProperty("hda-gfx", const_cast<char *>(hdaGfx), static_cast<uint32_t>(strlen(hdaGfx)+1));

	// Ensure built-in.
	if (!hdaService->getProperty("built-in")) {
		DBGLOG("alc", "fixing built-in");
		uint8_t builtBytes[] { 0x00 };
		hdaService->setProperty("built-in", builtBytes, sizeof(builtBytes));
	} else {
		DBGLOG("alc", "found existing built-in");
	}
}

void AlcEnabler::layoutLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context) {
	DBGLOG("alc", "layoutLoadCallback %u %d %d %u %d", requestTag, result, resourceData != nullptr, resourceDataLength, context != nullptr);
	callbackAlc->updateResource(Resource::Layout, result, resourceData, resourceDataLength);
	DBGLOG("alc", "layoutLoadCallback done %u %d %d %u %d", requestTag, result, resourceData != nullptr, resourceDataLength, context != nullptr);
	FunctionCast(layoutLoadCallback, callbackAlc->orgLayoutLoadCallback)(requestTag, result, resourceData, resourceDataLength, context);
}

void AlcEnabler::platformLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context) {
	DBGLOG("alc", "platformLoadCallback %u %d %d %u %d", requestTag, result, resourceData != nullptr, resourceDataLength, context != nullptr);
	callbackAlc->updateResource(Resource::Platform, result, resourceData, resourceDataLength);
	DBGLOG("alc", "platformLoadCallback done %u %d %d %u %d", requestTag, result, resourceData != nullptr, resourceDataLength, context != nullptr);
	FunctionCast(platformLoadCallback, callbackAlc->orgPlatformLoadCallback)(requestTag, result, resourceData, resourceDataLength, context);
}

IOService *AlcEnabler::gfxProbe(IOService *ctrl, IOService *provider, SInt32 *score) {
	auto name = provider->getName();
	DBGLOG("alc", "AppleGFXHDA probe for %s", safeString(name));

	if (name && !strcmp(name, "HDEF")) {
		// Starting with iMacPro custom audio cards are used on Apple hardware.
		// On MacBookPro15,x and newer devices these custom audio cards are controlled by T2, and
		// internal HDEF device is only used for HDMI audio output implemented via AppleGFXHDA kext,
		// which does not know about analog audio. AppleHDA still supports HDEF devices with analog
		// audio output as well as HDMI support for legacy devices, so we avoid AppleGFXHDA by all
		// means for HDEF.
		DBGLOG("alc", "avoiding AppleGFXHDA for HDEF device");
		return nullptr;
	}

	return FunctionCast(gfxProbe, callbackAlc->orgGfxProbe)(ctrl, provider, score);
}

uint32_t AlcEnabler::getAudioLayout(IOService *hdaDriver) {
	auto parent = hdaDriver->getParentEntry(gIOServicePlane);
	uint32_t layout = 0;
	while (parent) {
		auto name = parent->getName();
		if (name && (!strcmp(name, "HDEF") || !strcmp(name, "HDAU"))) {
			if (!WIOKit::getOSDataValue(parent, "layout-id", layout))
				SYSLOG("alc", "failed to obtain layout-id from %s", name);
			break;
		}
		parent = parent->getParentEntry(gIOServicePlane);
	}
	return layout;
}

IOReturn AlcEnabler::performPowerChange(IOService *hdaDriver, uint32_t from, uint32_t to, unsigned int *timer) {
	IOReturn ret = FunctionCast(performPowerChange, callbackAlc->orgPerformPowerChange)(hdaDriver, from, to, timer);

	auto hdaCodec = hdaDriver ? OSDynamicCast(IOService, hdaDriver->getParentEntry(gIOServicePlane)) : nullptr;
	if (hdaCodec) {
		auto pinStatus = OSDynamicCast(OSBoolean, hdaCodec->getProperty("alc-pinconfig-status"));
		auto sleepStatus = OSDynamicCast(OSBoolean, hdaCodec->getProperty("alc-sleep-status"));

		if (pinStatus && sleepStatus) {
			bool pin = pinStatus->getValue();
			bool sleep = sleepStatus->getValue();
			DBGLOG("alc", "power change %s at %s from %u to %u in from pin %d sleep %d",
				   safeString(hdaDriver->getName()), safeString(hdaCodec->getName()), from, to, pin, sleep);

			if (pin) {
				if (to == ALCAudioDeviceSleep) {
					hdaCodec->setProperty("alc-sleep-status", kOSBooleanTrue);
				} else if (sleep && (to == ALCAudioDeviceIdle || to == ALCAudioDeviceActive)) {
					DBGLOG("alc", "power change %s at %s forcing wake verbs", safeString(hdaDriver->getName()), safeString(hdaCodec->getName()));
					auto forceRet = FunctionCast(initializePinConfig, callbackAlc->orgInitializePinConfig)(hdaCodec, hdaCodec);
					SYSLOG_COND(forceRet != kIOReturnSuccess, "alc", "power change %s at %s forcing wake returned %08X",
								safeString(hdaDriver->getName()), safeString(hdaCodec->getName()), forceRet);
					hdaCodec->setProperty("alc-sleep-status", kOSBooleanFalse);
				}
			}

		} else {
			SYSLOG("alc", "power change failed to get pin %d sleep %d", pinStatus != nullptr, sleepStatus != nullptr);
		}

	} else {
		SYSLOG("alc", "power change failed to obtain hda codec");
	}

	return ret;
}

IOReturn AlcEnabler::initializePinConfig(IOService *hdaCodec, IOService *configDevice) {
	if (hdaCodec && configDevice && !hdaCodec->getProperty("alc-pinconfig-status")) {
		uint32_t appleLayout = getAudioLayout(hdaCodec);
		uint32_t analogCodec = 0;
		uint32_t analogLayout = 0;
		for (size_t i = 0, s = callbackAlc->codecs.size(); i < s; i++) {
			if (callbackAlc->controllers[callbackAlc->codecs[i]->controller]->layout > 0) {
				analogCodec = static_cast<uint32_t>(callbackAlc->codecs[i]->vendor) << 16 | callbackAlc->codecs[i]->codec;
				analogLayout = callbackAlc->controllers[callbackAlc->codecs[i]->controller]->layout;
				break;
			}
		}

		DBGLOG("alc", "initializePinConfig %s received hda " PRIKADDR ", config " PRIKADDR " config name %s apple layout %u codec %08X layout %u",
			   safeString(hdaCodec->getName()), CASTKADDR(hdaCodec), CASTKADDR(configDevice),
			   configDevice ? safeString(configDevice->getName()) : "(null config)", appleLayout, analogCodec, analogLayout);

		hdaCodec->setProperty("alc-pinconfig-status", kOSBooleanFalse);
		hdaCodec->setProperty("alc-sleep-status", kOSBooleanFalse);

		if (appleLayout && analogCodec && analogLayout) {
			auto configList = OSDynamicCast(OSArray, configDevice->getProperty("HDAConfigDefault"));
			if (configList) {
				unsigned int total = configList->getCount();
				DBGLOG("alc", "discovered HDAConfigDefault with %u entries", total);

				for (unsigned int i = 0; i < total; i++) {
					auto config = OSDynamicCast(OSDictionary, configList->getObject(i));
					if (config) {
						auto currCodec = OSDynamicCast(OSNumber, config->getObject("CodecID"));
						auto currLayout = OSDynamicCast(OSNumber, config->getObject("LayoutID"));
						if (currCodec && currLayout) {
							if (currCodec->unsigned32BitValue() == analogCodec && currLayout->unsigned32BitValue() == analogLayout) {
								auto configData = OSDynamicCast(OSData, config->getObject("ConfigData"));
								auto wakeConfigData = OSDynamicCast(OSData, config->getObject("WakeConfigData"));
								auto reinit = OSDynamicCast(OSBoolean, config->getObject("WakeVerbReinit"));
								DBGLOG("alc", "current config entry has boot %d, wake %d, reinit %d", configData != nullptr,
									   wakeConfigData != nullptr, reinit ? reinit->getValue() : -1);

								auto newConfig = OSDynamicCast(OSDictionary, config->copyCollection());
								if (newConfig) {
									// Replace the config list with a new list to avoid multiple iterations,
									// and actually fix the LayoutID number we hook in.
									auto num = OSNumber::withNumber(appleLayout, 32);
									if (num) {
										newConfig->setObject("LayoutID", num);
										num->release();
									}
									const OSObject *obj {OSDynamicCast(OSObject, newConfig)};
									auto arr = OSArray::withObjects(&obj, 1);
									if (arr) {
										configDevice->setProperty("HDAConfigDefault", arr);
										arr->release();
									}
									if (reinit && reinit->getValue()) {
										newConfig = OSDynamicCast(OSDictionary, newConfig->copyCollection());
										if (newConfig) {
											obj = OSDynamicCast(OSObject, newConfig);
											if (wakeConfigData) {
												if (configData)
													newConfig->setObject("BootConfigData", configData);
												newConfig->setObject("ConfigData", wakeConfigData);
												newConfig->removeObject("WakeConfigData");
											}
											// These will be same
											auto arr = OSArray::withObjects(&obj, 1);
											if (arr) {
												hdaCodec->setProperty("HDAConfigDefault", arr);
												hdaCodec->setProperty("alc-pinconfig-status", kOSBooleanTrue);
												arr->release();
											}
										} else {
											SYSLOG("alc", "failed to copy new HDAConfigDefault collection");
										}
									}
								} else {
									SYSLOG("alc", "failed to copy HDAConfigDefault %u collection", i);
								}

								break;
							}
						} else {
							SYSLOG("alc", "invalid CodecID %d or LayoutID %d at entry %u, pinconfigs are broken",
								   currCodec != nullptr, currLayout != nullptr, i);
						}
					} else {
						SYSLOG("alc", "invalid HDAConfigDefault entry at %u, pinconfigs are broken", i);
					}
				}
			} else {
				SYSLOG("alc", "invalid HDAConfigDefault, pinconfigs are broken");
			}

		}
	}

	return FunctionCast(initializePinConfig, callbackAlc->orgInitializePinConfig)(hdaCodec, configDevice);
}

void AlcEnabler::handleAudioClientEntitlement(task_t task, const char *entitlement, OSObject *&original) {
	if ((!original || original != kOSBooleanTrue) && !strcmp(entitlement, "com.apple.private.audio.driver-host"))
		original = kOSBooleanTrue;
}

void AlcEnabler::eraseRedundantLogs(KernelPatcher &patcher, size_t index) {
	static const uint8_t logAssertFind[] = { 0x53, 0x6F, 0x75, 0x6E, 0x64, 0x20, 0x61, 0x73 };
	static const uint8_t nullReplace[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	KernelPatcher::LookupPatch currentPatch {
		&ADDPR(kextList)[index], nullptr, nullReplace, sizeof(nullReplace)
	};

	if (index == KextIdAppleHDAController || index == KextIdAppleHDA) {
		currentPatch.find = logAssertFind;
		if (index == KextIdAppleHDAController)
			currentPatch.count = 3;
		else
			currentPatch.count = 2;

		patcher.applyLookupPatch(&currentPatch);
		patcher.clearError();
	}
}

void AlcEnabler::processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
	size_t kextIndex = 0;

	while (kextIndex < ADDPR(kextListSize)) {
		if (ADDPR(kextList)[kextIndex].loadIndex == index)
			break;
		kextIndex++;
	}
	
	if (kextIndex == ADDPR(kextListSize))
		return;

	if (kextIndex == KextIdAppleGFXHDA) {
		KernelPatcher::RouteRequest request("__ZN21AppleGFXHDAController5probeEP9IOServicePi", gfxProbe, orgGfxProbe);
		patcher.routeMultiple(index, &request, 1, address, size);
		return;
	}
	
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

			DBGLOG("alc", "handling %lu controller vendor %08X with %lu patches", i, info->vendor, info->patchNum);
			// Choose a free device-id for NVIDIA HDAU to support multigpu setups
			if (info->vendor == WIOKit::VendorID::NVIDIA) {
				for (size_t j = 0; j < info->patchNum; j++) {
					auto &p = info->patches[j].patch;
					if (p.size == sizeof(uint32_t) && *reinterpret_cast<const uint32_t *>(p.find) == NvidiaSpecialFind) {
						DBGLOG("alc", "finding %08X repl at %lu curr %lu", *reinterpret_cast<const uint32_t *>(p.replace), i, currentFreeNvidiaDeviceId);
						while (currentFreeNvidiaDeviceId < MaxNvidiaDeviceIds) {
							if (!nvidiaDeviceIdUsage[currentFreeNvidiaDeviceId]) {
								p.find = reinterpret_cast<const uint8_t *>(&nvidiaDeviceIdList[currentFreeNvidiaDeviceId]);
								DBGLOG("alc", "assigned %08X find %08X repl at %lu curr %lu", *reinterpret_cast<const uint32_t *>(p.find), *reinterpret_cast<const uint32_t *>(p.replace), i, currentFreeNvidiaDeviceId);
								nvidiaDeviceIdUsage[currentFreeNvidiaDeviceId] = true;
								currentFreeNvidiaDeviceId++;
								break;
							}
							currentFreeNvidiaDeviceId++;
						}
					}
				}
			}

			if (controllers[i]->nopatch) {
				DBGLOG("alc", "skipping %lu controller %X:%X:%X due to no-controller-patch", i, controllers[i]->vendor, controllers[i]->device, controllers[i]->revision);
				continue;
			}
			applyPatches(patcher, index, info->patches, info->patchNum);
		}

		// Only do this if -alcdbg is not passed
		if (!ADDPR(debugEnabled))
			eraseRedundantLogs(patcher, kextIndex);
	}
	
	if (progressState & ProcessingState::CodecsLoaded) {
		for (size_t i = 0, num = codecs.size(); i < num; i++) {
			auto &info = codecs[i]->info;
			if (!info) {
				SYSLOG("alc", "missing CodecModInfo for %lu codec", i);
				continue;
			}
			
			if (info->platformNum > 0 || info->layoutNum > 0) {
				DBGLOG("alc", "will route resource loading callbacks");
				progressState |= ProcessingState::CallbacksWantRouting;
			}
			
			applyPatches(patcher, index, info->patches, info->patchNum);
		}
	}
	
	if ((progressState & ProcessingState::CallbacksWantRouting) && kextIndex == KextIdAppleHDA) {
		KernelPatcher::RouteRequest requests[] {
			KernelPatcher::RouteRequest("__ZN14AppleHDADriver18layoutLoadCallbackEjiPKvjPv", layoutLoadCallback, orgLayoutLoadCallback),
			KernelPatcher::RouteRequest("__ZN14AppleHDADriver20platformLoadCallbackEjiPKvjPv", platformLoadCallback, orgPlatformLoadCallback),
			KernelPatcher::RouteRequest("__ZN14AppleHDADriver23performPowerStateChangeE24_IOAudioDevicePowerStateS0_Pj", performPowerChange, orgPerformPowerChange),
			KernelPatcher::RouteRequest("__ZN20AppleHDACodecGeneric38initializePinConfigDefaultFromOverrideEP9IOService", initializePinConfig, orgInitializePinConfig),
		};

		patcher.routeMultiple(index, requests, address, size);

		// patch AppleHDA to remove redundant logs
		if (!ADDPR(debugEnabled))
			eraseRedundantLogs(patcher, kextIndex);
	}
	
	// Ignore all the errors for other processors
	patcher.clearError();
}

void AlcEnabler::updateResource(Resource type, kern_return_t &result, const void * &resourceData, uint32_t &resourceDataLength) {
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
				if (controllers[codecs[i]->controller]->layout == fi.layout && KernelPatcher::compatibleKernel(fi.minKernel, fi.maxKernel)) {
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
	auto sectPCI = WIOKit::findEntryByPrefix("/AppleACPIPlatformExpert", "PCI", gIOServicePlane);

	for (size_t lookup = 0; lookup < ADDPR(codecLookupSize); lookup++) {
		auto sect = sectPCI;
		
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
				
				if (ADDPR(codecLookup)[lookup].detect && !WIOKit::getOSDataValue(sect, "alc-layout-id", lid)) {
					SYSLOG("alc", "alc-layout-id was not provided by controller at %s", ADDPR(codecLookup)[lookup].tree[i]);
					break;
				}
				
				if (WIOKit::getOSDataValue(sect, "AAPL,ig-platform-id", platform)) {
					DBGLOG("alc", "AAPL,ig-platform-id %X was found in controller at %s", platform, ADDPR(codecLookup)[lookup].tree[i]);
				} else if (WIOKit::getOSDataValue(sect, "AAPL,snb-platform-id", platform)) {
					DBGLOG("alc", "AAPL,snb-platform-id %X was found in controller at %s", platform, ADDPR(codecLookup)[lookup].tree[i]);
				}

				insertController(ven, dev, rev, platform, nullptr != sect->getProperty("no-controller-patch"), lid, ADDPR(codecLookup)[lookup].detect, &ADDPR(codecLookup)[lookup]);
				break;
			}
		}
	}
	
	if (controllers.size() > 0) {
		DBGLOG("alc", "found %lu audio controllers", controllers.size());
		validateControllers();
	}
}

bool AlcEnabler::appendCodec(void *user, IORegistryEntry *e) {
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

	auto ci = AlcEnabler::CodecInfo::create(alc->currentController, venNum->unsigned32BitValue(), revNum->unsigned32BitValue());
	if (ci) {
		if (!alc->codecs.push_back(ci)) {
			SYSLOG("alc", "failed to store codec info for %X:%X:%X", ci->vendor, ci->codec, ci->revision);
			AlcEnabler::CodecInfo::deleter(ci);
		}
	} else {
		SYSLOG("alc", "failed to create codec info for %X %X:%X", ci->vendor, ci->codec, ci->revision);
	}

	return true;
}

bool AlcEnabler::grabCodecs() {
	for (currentController = 0; currentController < controllers.size(); currentController++) {
		auto ctlr = controllers[currentController];

		// Digital controllers normally have no detectible codecs
		if (!ctlr->detect || !ctlr->lookup)
			continue;

		auto sect = WIOKit::findEntryByPrefix("/AppleACPIPlatformExpert", "PCI", gIOServicePlane);

		for (size_t i = 0; sect && i < ctlr->lookup->treeSize; i++) {
			bool last = i+1 == ctlr->lookup->treeSize;
			sect = WIOKit::findEntryByPrefix(sect, ctlr->lookup->tree[i], gIOServicePlane,
											 last ? appendCodec : nullptr, last, this);
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
