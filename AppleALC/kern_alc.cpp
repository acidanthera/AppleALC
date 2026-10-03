//
//  kern_alc.cpp
//  AppleALC
//
//  Copyright © 2016-2017 vit9696. All rights reserved.
//

#include <Headers/kern_api.hpp>
#include <Headers/kern_devinfo.hpp>
#include <Headers/plugin_start.hpp>
#include <Headers/kern_compression.hpp>
#include <IOKit/IOService.h>
#include <IOKit/pci/IOPCIDevice.h>
#include <mach/vm_map.h>
#include <libkern/c++/OSUnserialize.h>

#include "kern_alc.hpp"
#include "kern_resources.hpp"

static AlcEnabler alcEnabler;

// Only used in apple-driven callbacks
AlcEnabler* AlcEnabler::callbackAlc = nullptr;

void AlcEnabler::createShared() {
	if (callbackAlc)
		PANIC("alc", "Attempted to assign alc callback again");
	
	callbackAlc = &alcEnabler;
	
	if (!callbackAlc)
		PANIC("alc", "Failed to assign alc callback");
}

void AlcEnabler::init() {

	lilu.onPatcherLoadForce(
	[](void *user, KernelPatcher &patcher) {
		static_cast<AlcEnabler *>(user)->updateProperties();
	}, this);

#ifdef HAVE_ANALOG_AUDIO
	if (getKernelVersion() < KernelVersion::Mojave)
		ADDPR(kextList)[KextIdAppleGFXHDA].switchOff();
	if (getKernelVersion() == KernelVersion::Tiger || getKernelVersion() >= KernelVersion::Lion)
		ADDPR(kextList)[KextIdAppleHDAPlatformDriver].switchOff();
#else
	ADDPR(kextList)[KextIdAppleGFXHDA].switchOff();
	ADDPR(kextList)[KextIdAppleHDA].switchOff();
	ADDPR(kextList)[KextIdAppleHDAPlatformDriver].switchOff();
#endif

	lilu.onKextLoadForce(ADDPR(kextList), ADDPR(kextListSize),
	[](void *user, KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size) {
		static_cast<AlcEnabler *>(user)->processKext(patcher, index, address, size);
	}, this);

	if (getKernelVersion() >= KernelVersion::Sierra) {
		// Unlock custom audio engines by disabling Apple private entitlement verification
		// Recent macOS versions (e.g. 10.13.6) support legacy_hda_tools_support=1 boot argument, which works similarly.
		if (checkKernelArgument("-alcdhost")) {
			if (getKernelVersion() >= KernelVersion::HighSierra)
				SYSLOG("alc", "consider replacing -alcdhost with legacy_hda_tools_support=1 boot-arg!");
			lilu.onEntitlementRequestForce([](void *user, task_t task, const char *entitlement, OSObject *&original) {
				static_cast<AlcEnabler *>(user)->handleAudioClientEntitlement(task, entitlement, original);
			}, this);
		}
	}
}

void AlcEnabler::deinit() {
	controllers.deinit();
#ifdef HAVE_ANALOG_AUDIO
	codecs.deinit();
#endif
}

void AlcEnabler::updateProperties() {
	auto devInfo = DeviceInfo::create();
	if (devInfo) {
		// Assume that IGPU with connections means built-in digital audio.
		bool hasBuiltinDigitalAudio = !devInfo->reportedFramebufferIsConnectorLess && devInfo->videoBuiltin;

		// Respect desire to disable digital audio. This may be particularly useful for configurations
		// with broken digital audio, resulting in kernel panics. Ref: https://github.com/acidanthera/bugtracker/issues/513
		if (hasBuiltinDigitalAudio && devInfo->audioBuiltinAnalog && devInfo->audioBuiltinAnalog->getProperty("No-hda-gfx"))
			hasBuiltinDigitalAudio = false;

		// Firstly, update Haswell or Broadwell HDAU device for built-in digital audio.
		if (devInfo->audioBuiltinDigital && validateInjection(devInfo->audioBuiltinDigital)) {
			if (hasBuiltinDigitalAudio) {
				// This is a normal HDAU device for an IGPU with connectors.
				updateDeviceProperties(devInfo->audioBuiltinDigital, devInfo, "onboard-1", false);
				uint32_t dev = 0, rev = 0;
				if (WIOKit::getOSDataValue(devInfo->audioBuiltinDigital, "device-id", dev) &&
					WIOKit::getOSDataValue(devInfo->audioBuiltinDigital, "revision-id", rev))
					insertController(WIOKit::VendorID::Intel, dev, rev, nullptr != devInfo->audioBuiltinDigital->getProperty("no-controller-patch"), devInfo->reportedFramebufferId);
			} else {
				// Terminate built-in HDAU audio, as we are using no connectors!
				WIOKit::awaitPublishing(devInfo->audioBuiltinDigital);
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

#ifdef HAVE_ANALOG_AUDIO
		// Secondly, update HDEF device and make it support digital audio
		if (devInfo->audioBuiltinAnalog && validateInjection(devInfo->audioBuiltinAnalog)) {
			uint32_t ven = 0;
			if (WIOKit::getOSDataValue(devInfo->audioBuiltinAnalog, "vendor-id", ven) && ven == WIOKit::VendorID::Intel) {
				uint32_t updateTcsel = 0;
				if (!lilu_get_boot_args("alctcsel", &updateTcsel, sizeof(updateTcsel)) &&
					!WIOKit::getOSDataValue(devInfo->audioBuiltinAnalog, "alctcsel", updateTcsel)) {
					updateTcsel = 0;
				}
				if (updateTcsel != 0) {
					// Intentionally using static cast to avoid PCI imports.
					WIOKit::awaitPublishing(devInfo->audioBuiltinAnalog);
					auto hdef = static_cast<IOPCIDevice *>(devInfo->audioBuiltinAnalog->metaCast("IOPCIDevice"));
					if (hdef != nullptr) {
						// Update Traffic Class Select Register to TC0.
						// This is required for AppleHDA to output audio on some machines.
						// See Intel I/O Controller Hub 9 (ICH9) Family Datasheet for more details.
						static constexpr size_t RegTCSEL = 0x44;
						auto value = hdef->configRead8(RegTCSEL);
						DBGLOG("alc", "updating TCSEL register %X", value);
						hdef->configWrite8(RegTCSEL, getBitField<uint8_t>(value, 7, 3));
					} else {
						SYSLOG("alc", "cannot access HDEF pci");
					}
				} else {
					DBGLOG("alc", "disabling TCSEL update");
				}
			}

			const char *hdaGfx = nullptr;
			if (hasBuiltinDigitalAudio && !devInfo->audioBuiltinDigital)
				hdaGfx = "onboard-1";
			updateDeviceProperties(devInfo->audioBuiltinAnalog, devInfo, hdaGfx, true);
		}
#endif

		// Thirdly, update IGPU device in case we have digital audio
		if (hasBuiltinDigitalAudio && validateInjection(devInfo->videoBuiltin)) {
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
			auto hdaService = devInfo->videoExternal[gpu].audio;
			auto gpuService = devInfo->videoExternal[gpu].video;

			if (!hdaService || !validateInjection(hdaService))
				continue;

			uint32_t ven = devInfo->videoExternal[gpu].vendor;
			uint32_t dev = 0, rev = 0;
			if (WIOKit::getOSDataValue(hdaService, "device-id", dev) &&
				WIOKit::getOSDataValue(hdaService, "revision-id", rev)) {
				// Register the controller
				insertController(ven, dev, rev, nullptr != hdaService->getProperty("no-controller-patch"));
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
			updateDeviceProperties(hdaService, devInfo, hdaGfx, false);
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

			// Check that we allow sending verbs.
			uint32_t enableHdaVerbs = 0;
			uint32_t enableAlcDelay = 0;
			bool checkVerbs = !lilu_get_boot_args("alcverbs", &enableHdaVerbs, sizeof(enableHdaVerbs));
			bool checkDelay = !lilu_get_boot_args("alcdelay", &enableAlcDelay, sizeof(enableAlcDelay));

			if (checkVerbs || checkDelay) {
				if (devInfo->audioBuiltinAnalog) {
					if (checkVerbs && devInfo->audioBuiltinAnalog->getProperty("alc-verbs")) {
						enableHdaVerbs = 1;
						checkVerbs = false;
					}
					if (checkDelay && devInfo->audioBuiltinAnalog->getProperty("alc-delay")) {
						enableAlcDelay = 1;
						checkDelay = false;
					}
				}

				for (size_t gpu = 0; gpu < devInfo->videoExternal.size(); gpu++) {
					auto hdaService = devInfo->videoExternal[gpu].audio;
					if (!hdaService) continue;
					if (checkVerbs && hdaService->getProperty("alc-verbs")) {
						enableHdaVerbs = 1;
						checkVerbs = false;
					}

					if (checkDelay && hdaService->getProperty("alc-delay")) {
						enableAlcDelay = 1;
						checkDelay = false;
					}
				}
			}

			if (!enableHdaVerbs) {
				DBGLOG("alc", "no verb support requested, disabling");
				ADDPR(kextList)[KextIdIOHDAFamily].switchOff();
			}

			if (enableAlcDelay) {
				DBGLOG("alc", "has delay support requested, enabling");
			} else {
				progressState |= ProcessingState::PatchHDAController;
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

#ifdef HAVE_ANALOG_AUDIO
	if (isAnalog) {
		// Refresh our own layout-id named alc-layout-id as follows:
		// alcid=X has highest priority and overrides any other value.
		// alc-layout-id has normal priority and is expected to be used.
		// layout-id will be used if both alcid and alc-layout-id are not set on non-Apple platforms.
		uint32_t layout = 0;
		if (lilu_get_boot_args("alcid", &layout, sizeof(layout))) {
			DBGLOG("alc", "found alc-layout-id override %u", layout);
			hdaService->setProperty("alc-layout-id", &layout, sizeof(layout));
		} else {
			uint32_t alcId;
			if (info->firmwareVendor == DeviceInfo::FirmwareVendor::Apple &&
				WIOKit::getOSDataValue(hdaService, "alc-layout-id", alcId)) {
				DBGLOG("alc", "found apple alc-layout-id %u property", alcId);
			} else if (info->firmwareVendor != DeviceInfo::FirmwareVendor::Apple
					   || hdaService->getProperty("use-layout-id") != nullptr) {
				if (WIOKit::getOSDataValue(hdaService, "layout-id", alcId)) {
					DBGLOG("alc", "found legacy layout-id %u property", alcId);
					hdaService->setProperty("alc-layout-id", &alcId, sizeof(alcId));
				} else {
					SYSLOG("alc", "error: no layout-id property found in configuration");
				}
			}
		}

		// SystemAudioVolume variable used by boot chime sound will be capped by this value.
		// Only lower 7 bits are valid bits for volume level, 8th bit is used for muted status.
		if (!hdaService->getProperty("MaximumBootBeepVolume")) {
			DBGLOG("alc", "fixing MaximumBootBeepVolume in hdef");
			uint8_t bootBeepBytes[] { 0x7F };
			hdaService->setProperty("MaximumBootBeepVolume", bootBeepBytes, sizeof(bootBeepBytes));
		}

		if (!hdaService->getProperty("MaximumBootBeepVolumeAlt")) {
			DBGLOG("alc", "fixing MaximumBootBeepVolumeAlt in hdef");
			uint8_t bootBeepBytes[] { 0x7F };
			hdaService->setProperty("MaximumBootBeepVolumeAlt", bootBeepBytes, sizeof(bootBeepBytes));
		}

		if (!hdaService->getProperty("PinConfigurations")) {
			DBGLOG("alc", "fixing PinConfigurations in hdef");
			uint8_t pinBytes[] { 0x00 };
			hdaService->setProperty("PinConfigurations", pinBytes, sizeof(pinBytes));
		}
	}
#else
	assert(isAnalog == false);
#endif

	// For every client only set layout-id itself.
	if (info->firmwareVendor != DeviceInfo::FirmwareVendor::Apple || hdaService->getProperty("use-apple-layout-id") != nullptr) {
		hdaService->setProperty("layout-id", &info->reportedLayoutId, sizeof(info->reportedLayoutId));
#ifdef HAVE_ANALOG_AUDIO
		layoutIdIsOverridden = true;
		layoutIdOverride = info->reportedLayoutId;
#endif
	}

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

bool AlcEnabler::AppleHDAController_start(IOService* service, IOService* provider)
{
	uint32_t delay = 0;
	if (lilu_get_boot_args("alcdelay", &delay, sizeof(delay))) {
		DBGLOG("alc", "found alc-delay override %u", delay);
		provider->setProperty("alc-delay", &delay, sizeof(delay));
	} else {
		if (WIOKit::getOSDataValue(provider, "alc-delay", delay))
			DBGLOG("alc", "found normal alc-delay %u", delay);
	}
	
	if (delay > 3000) {
		SYSLOG("alc", "alc delay cannot exceed 3000 ms, ignore it");
		delay = 0;
	}
		
	if (delay != 0) {
		DBGLOG("alc", "delay AppleHDAController::start for %d ms", delay);
		IOSleep(delay);
	}
	return FunctionCast(AppleHDAController_start, callbackAlc->orgAppleHDAController_start)(service, provider);
}

IOReturn AlcEnabler::IOHDACodecDevice_executeVerb(void *hdaCodecDevice, uint16_t nid, uint16_t verb, uint16_t param, unsigned int *output, bool waitForSuccess)
{
	if (verb & 0xff0) {
		// 12 bit verb
		DBGLOG("alc", "IOHDACodecDevice::executeVerb with parameters nid = 0x%02X, verb = 0x%03X, param = 0x%02X", nid, verb, param);
	} else {
		// 4 bit verb
		DBGLOG("alc", "IOHDACodecDevice::executeVerb with parameters nid = 0x%02X, verb = 0x%X, param = 0x%04X", nid, verb, param);
	}
	return FunctionCast(IOHDACodecDevice_executeVerb, callbackAlc->orgIOHDACodecDevice_executeVerb)(hdaCodecDevice, nid, verb, param, output, waitForSuccess);
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

#ifdef HAVE_ANALOG_AUDIO
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
#else
	if (!(progressState & ProcessingState::ControllersLoaded)) {
		grabControllers();
		progressState |= ProcessingState::ControllersLoaded;
	}
#endif


	// Continue to patch controllers
	
	if (progressState & ProcessingState::ControllersLoaded) {
		for (size_t i = 0, num = controllers.size(); i < num; i++) {
			auto info = controllers[i]->info;
			if (!info) {
				DBGLOG("alc", "missing ControllerModInfo for %lu controller", i);
				continue;
			}

			DBGLOG("alc", "handling %lu controller %X:%X with %lu patches - %s", i, info->vendor, info->device, info->patchNum, info->name);
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

#ifdef HAVE_ANALOG_AUDIO
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
		// AppleHDADriver::performPowerStateChange
		KernelPatcher::RouteRequest requestPowerChange(symPerformPowerChange, performPowerChange, orgPerformPowerChange);
		patcher.routeMultiple(index, &requestPowerChange, 1, address, size);
		
		// AppleHDACodecGeneric::initializePinConfigDefaultFromOverride does not take an IOService parameter in most versions of 10.5 and under.
		if (getKernelVersion() >= KernelVersion::SnowLeopard
				|| patcher.solveSymbol(index, "__ZN20AppleHDACodecGeneric38initializePinConfigDefaultFromOverrideEP9IOService")) {
			KernelPatcher::RouteRequest requestPinConfig("__ZN20AppleHDACodecGeneric38initializePinConfigDefaultFromOverrideEP9IOService", initializePinConfig, orgInitializePinConfig);
			patcher.routeMultiple(index, &requestPinConfig, 1, address, size);
		} else {
			patcher.clearError();
			KernelPatcher::RouteRequest requestPinConfig("__ZN20AppleHDACodecGeneric38initializePinConfigDefaultFromOverrideEv", initializePinConfigLegacy, orgInitializePinConfigLegacy);
			patcher.routeMultiple(index, &requestPinConfig, 1, address, size);
		}
		
		// layout and platform load callbacks only exist in 10.6.8 and later
		if (patcher.solveSymbol(index, "__ZN14AppleHDADriver18layoutLoadCallbackEjiPKvjPv")) {
			KernelPatcher::RouteRequest requestsCallbacks[] {
				KernelPatcher::RouteRequest("__ZN14AppleHDADriver18layoutLoadCallbackEjiPKvjPv", layoutLoadCallback, orgLayoutLoadCallback),
				KernelPatcher::RouteRequest("__ZN14AppleHDADriver20platformLoadCallbackEjiPKvjPv", platformLoadCallback, orgPlatformLoadCallback)
			};
			patcher.routeMultiple(index, requestsCallbacks, address, size);
		} else {
			patcher.clearError();
		}
		
		// 10.6.8 to 10.7.5, and early versions of 10.8 do not use zlib compression for resources
		isAppleHDAZlib = getKernelVersion() >= KernelVersion::Mavericks || patcher.solveSymbol(index, "__Z24AppleHDA_zlib_uncompressPhPmPKhm") != 0;
		if (!isAppleHDAZlib)
			patcher.clearError();

		// 10.4 contains the platforms and layouts in AppleHDA directly
		if (getKernelVersion() == KernelVersion::Tiger) {
			KernelPatcher::RouteRequest request("__ZN14AppleHDADriver5startEP9IOService", AppleHDADriver_start, orgAppleHDADriver_start);
			patcher.routeMultiple(index, &request, 1, address, size);
		}

		// patch AppleHDA to remove redundant logs
		if (!ADDPR(debugEnabled))
			eraseRedundantLogs(patcher, kextIndex);
	}
#endif

	if (!(progressState & ProcessingState::PatchHDAFamily) && kextIndex == KextIdIOHDAFamily) {
		progressState |= ProcessingState::PatchHDAFamily;
		KernelPatcher::RouteRequest request(symIOHDACodecDevice_executeVerb, IOHDACodecDevice_executeVerb, orgIOHDACodecDevice_executeVerb);
		patcher.routeMultiple(index, &request, 1, address, size);
	}
	
	if (!(progressState & ProcessingState::PatchHDAController) && kextIndex == KextIdAppleHDAController) {
		progressState |= ProcessingState::PatchHDAController;
		KernelPatcher::RouteRequest request("__ZN18AppleHDAController5startEP9IOService", AppleHDAController_start, orgAppleHDAController_start);
		patcher.routeMultiple(index, &request, 1, address, size);
	}
	
#ifdef HAVE_ANALOG_AUDIO
	// Layout/platform info is in AppleHDAPlatformDriver on versions 10.5.x to 10.6.7
	if (!(progressState & ProcessingState::PatchHDAPlatformDriver) && kextIndex == KextIdAppleHDAPlatformDriver) {
		progressState |= ProcessingState::PatchHDAPlatformDriver;
		KernelPatcher::RouteRequest request("__ZN22AppleHDAPlatformDriver5startEP9IOService", AppleHDAPlatformDriver_start, orgAppleHDAPlatformDriver_start);
		patcher.routeMultiple(index, &request, 1, address, size);
	}
#endif

	// Ignore all the errors for other processors
	patcher.clearError();
}

void AlcEnabler::grabControllers() {
	computerModel = BaseDeviceInfo::get().modelType;

	auto devInfo = DeviceInfo::create();
	if (devInfo) {
		// Nice, we found some controller, add it
		uint32_t ven {0}, dev {0}, rev {0}, lid {0};
		auto sect = devInfo->audioBuiltinAnalog;
		if (sect &&
			WIOKit::getOSDataValue(sect, "vendor-id", ven) &&
			WIOKit::getOSDataValue(sect, "device-id", dev) &&
			WIOKit::getOSDataValue(sect, "revision-id", rev) &&
			WIOKit::getOSDataValue(sect, "alc-layout-id", lid)) {

			insertController(ven, dev, rev, ControllerModInfo::PlatformAny, nullptr != sect->getProperty("no-controller-patch"), lid, sect);
		} else {
			SYSLOG("alc", "failed to obtain device info for analog controller (%d)", devInfo->audioBuiltinAnalog != nullptr);
		}

		DeviceInfo::deleter(devInfo);
	} else {
		SYSLOG("alc", "failed to obtain device info for analog controller");
	}

	if (controllers.size() > 0) {
		DBGLOG("alc", "found %lu audio controllers", controllers.size());
		validateControllers();
	}
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
					DBGLOG("alc", "not matching platform was found %X vs %X for %s", ADDPR(controllerMod)[mod].platform, controllers[i]->platform, &ADDPR(controllerMod)[mod].name);
					continue;
				}

				// Check if computer model is suitable
				if (!(computerModel & ADDPR(controllerMod)[mod].computerModel)) {
					DBGLOG("alc", "unsuitable computer model was found %X vs %X for %s", ADDPR(controllerMod)[mod].computerModel, computerModel, &ADDPR(controllerMod)[mod].name);
					continue;
				}

				if (rev != ADDPR(controllerMod)[mod].revisionNum ||
					ADDPR(controllerMod)[mod].revisionNum == 0) {
					DBGLOG("alc", "found mod for %lu controller - %s", i, &ADDPR(controllerMod)[mod].name);
					controllers[i]->info= &ADDPR(controllerMod)[mod];
					break;
				}
			}
		}
	}
}

#ifdef HAVE_ANALOG_AUDIO
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

void AlcEnabler::patchPinConfig(IOService *hdaCodec, IORegistryEntry *configDevice) {
	if (hdaCodec && configDevice && !hdaCodec->getProperty("alc-pinconfig-status")) {
		uint32_t appleLayout = getAudioLayout(hdaCodec);
		uint32_t analogCodec = 0;
		uint32_t analogLayout = 0;
		for (size_t i = 0, s = codecs.size(); i < s; i++) {
			if (controllers[codecs[i]->controller]->layout > 0) {
				analogCodec = static_cast<uint32_t>(codecs[i]->vendor) << 16 | codecs[i]->codec;
				analogLayout = controllers[codecs[i]->controller]->layout;
				break;
			}
		}

		DBGLOG("alc", "initializePinConfig %s received hda " PRIKADDR ", config " PRIKADDR " config name %s apple layout %u codec %08X layout %u",
			safeString(hdaCodec->getName()), CASTKADDR(hdaCodec), CASTKADDR(configDevice),
			configDevice ? safeString(configDevice->getName()) : "(null config)", appleLayout, analogCodec, analogLayout);

		hdaCodec->setProperty("alc-pinconfig-status", kOSBooleanFalse);
		hdaCodec->setProperty("alc-sleep-status", kOSBooleanFalse);

		auto alcSelf = ADDPR(selfInstance);
		if (!alcSelf) {
			SYSLOG("alc", "invalid self reference");
			return;
		}

		if (appleLayout && analogCodec && analogLayout) {
			auto configList = OSDynamicCast(OSArray, alcSelf->getProperty("HDAConfigDefault"));
			if (configList) {
				unsigned int total = configList->getCount();
				DBGLOG("alc", "discovered HDAConfigDefault with %u entries", total);

				for (unsigned int i = 0; i < total; i++) {
					auto config = OSDynamicCast(OSDictionary, configList->getObject(i));
					if (config == nullptr) {
						SYSLOG("alc", "invalid HDAConfigDefault entry at %u, pinconfigs are broken", i);
						continue;
					}
					auto currCodec = OSDynamicCast(OSNumber, config->getObject("CodecID"));
					auto currLayout = OSDynamicCast(OSNumber, config->getObject("LayoutID"));
					if (currCodec == nullptr || currLayout == nullptr ||
						currCodec->unsigned32BitValue() != analogCodec || currLayout->unsigned32BitValue() != analogLayout) {
						// Not analog or wrong entry.
						continue;
					}

					auto newConfigCollection = config->copyCollection();
					auto newConfig = OSDynamicCast(OSDictionary, newConfigCollection);
					const OSObject *newConfigObj  = OSDynamicCast(OSObject, newConfigCollection);
					if (newConfig == nullptr || newConfigObj == nullptr) {
						SYSLOG("alc", "failed to copy analog HDAConfigDefault %u collection", i);
						OSSafeReleaseNULL(newConfigCollection);
						break;
					}

					auto configData = OSDynamicCast(OSData, config->getObject("ConfigData"));
					auto wakeConfigData = OSDynamicCast(OSData, config->getObject("WakeConfigData"));
					auto reinitBool = OSDynamicCast(OSBoolean, config->getObject("WakeVerbReinit"));
					auto reinit = reinitBool != nullptr ? reinitBool->getValue() : false;
					DBGLOG("alc", "current config entry has boot %d, wake %d, reinit %d", configData != nullptr,
						wakeConfigData != nullptr, reinitBool ? reinit : -1);

					// Replace the config list with a new list to avoid multiple iterations,
					// and actually fix the LayoutID number we hook in.
					auto num = OSNumber::withNumber(appleLayout, 32);
					if (num != nullptr) {
						newConfig->setObject("LayoutID", num);
						num->release();
					}

					const OSObject *objForArr = newConfigObj;
					auto arr = OSArray::withObjects(&objForArr, 1);
					if (arr != nullptr) {
						configDevice->setProperty("HDAConfigDefault", arr);
						newConfig->retain();
						arr->release();
					}

					if (!reinit) {
						// We do not need to reinit, thus are done.
						newConfig->release();
						break;
					}

					newConfigCollection = newConfig->copyCollection();
					newConfig->release();
					newConfig = OSDynamicCast(OSDictionary, newConfigCollection);
					newConfigObj  = OSDynamicCast(OSObject, newConfigCollection);
					if (newConfig == nullptr || newConfigObj == nullptr) {
						SYSLOG("alc", "failed to copy new HDAConfigDefault collection for reinit");
						OSSafeReleaseNULL(newConfigCollection);
						break;
					}

					if (wakeConfigData != nullptr) {
						if (configData != nullptr) {
							newConfig->setObject("BootConfigData", configData);
						}
						newConfig->setObject("ConfigData", wakeConfigData);
						newConfig->removeObject("WakeConfigData");
					}
					objForArr = newConfigObj;
					arr = OSArray::withObjects(&objForArr, 1);
					if (arr != nullptr) {
						hdaCodec->setProperty("HDAConfigDefault", arr);
						hdaCodec->setProperty("alc-pinconfig-status", kOSBooleanTrue);
						arr->release();
					} else {
						newConfig->release();
					}

					break;
				}
			} else {
				SYSLOG("alc", "invalid HDAConfigDefault, pinconfigs are broken");
			}
		}
	}
}

IOReturn AlcEnabler::initializePinConfigLegacy(IOService *hdaCodec) {
	auto parentDevice = hdaCodec->getParentEntry(gIOServicePlane);
	while (parentDevice) {
		auto name = parentDevice->getName();
		if (name && (!strcmp(name, "AppleHDAController"))) {
			break;
		}

		parentDevice = parentDevice->getParentEntry(gIOServicePlane);
	}

	if (parentDevice)
		callbackAlc->patchPinConfig(hdaCodec, parentDevice);
	else
		SYSLOG("alc", "failed to get parent AppleHDAController instance");
	return FunctionCast(initializePinConfigLegacy, callbackAlc->orgInitializePinConfigLegacy)(hdaCodec);
}

IOReturn AlcEnabler::initializePinConfig(IOService *hdaCodec, IOService *configDevice) {
	callbackAlc->patchPinConfig(hdaCodec, configDevice);
	return FunctionCast(initializePinConfig, callbackAlc->orgInitializePinConfig)(hdaCodec, configDevice);
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
					DBGLOG("alc", "found %s at %lu index, zlib %u", type == Resource::Platform ? "platform" : "layout", f, isAppleHDAZlib);
					
					// decompress resource for non-zlib systems
					if (!isAppleHDAZlib) {
						uint32_t bufferLength = 0x7A000; // Buffer size that AppleHDA uses.
						auto buffer = Compression::decompress(Compression::ModeZLIB, &bufferLength, fi.data, fi.dataLength, nullptr);
						if (!buffer) {
							break;
						}
						
						resourceData = buffer;
						resourceDataLength = bufferLength;

					} else {
						resourceData = fi.data;
						resourceDataLength = fi.dataLength;
					}
					result = kOSReturnSuccess;
					break;
				}
			}
		}
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
		DBGLOG("alc", "storing codec info for %X:%X:%X", ci->vendor, ci->codec, ci->revision);
		if (!alc->codecs.push_back(ci)) {
			SYSLOG("alc", "failed to store codec info for %X:%X:%X", ci->vendor, ci->codec, ci->revision);
			AlcEnabler::CodecInfo::deleter(ci);
		}
	} else {
		SYSLOG("alc", "failed to create codec info for %X:%X", venNum->unsigned32BitValue(), revNum->unsigned32BitValue());
	}

	return true;
}

bool AlcEnabler::grabCodecs() {
	for (currentController = 0; currentController < controllers.size(); currentController++) {
		auto ctlr = controllers[currentController];

		// Digital controllers normally have no detectible codecs
		if (!ctlr->detect)
			continue;

		bool found = false;
		for (size_t brute = 0; !found && brute < WIOKit::bruteMax; brute++) {
			auto iterator = IORegistryIterator::iterateOver(ctlr->detect, gIOServicePlane, kIORegistryIterateRecursively);
			if (iterator) {
				IORegistryEntry *codec = nullptr;
				while ((codec = OSDynamicCast(IORegistryEntry, iterator->getNextObject())) != nullptr) {
					if (codec->getProperty("IOHDACodecVendorID")) {
						DBGLOG("alc", "found analog codec %s", safeString(codec->getName()));
						found = appendCodec(this, codec);
						break;
					}
				}

				iterator->release();
			}

			SYSLOG_COND(ADDPR(debugEnabled), "alc", "failed to find IOHDACodecVendorID, retrying %lu", brute);
		}
	}

	return validateCodecs();
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

bool AlcEnabler::AppleHDADriver_start(IOService *service, IOService *provider) {
	callbackAlc->replaceAppleHDADriverResources(service);
	
	return FunctionCast(AppleHDADriver_start, callbackAlc->orgAppleHDADriver_start)(service, provider);
}

bool AlcEnabler::AppleHDAPlatformDriver_start(IOService* service, IOService* provider) {
	callbackAlc->replaceAppleHDADriverResources(service);
	
	return FunctionCast(AppleHDAPlatformDriver_start, callbackAlc->orgAppleHDAPlatformDriver_start)(service, provider);
}

void AlcEnabler::replaceAppleHDADriverResources(IOService *service) {
	DBGLOG("alc", "replacing AppleHDA legacy resources");
	
	auto pathMapsDriverArray = OSArray::withCapacity((uint32_t) codecs.size());
	auto layoutsDriverArray = OSArray::withCapacity((uint32_t) codecs.size());
	if (!pathMapsDriverArray || !layoutsDriverArray) {
		SYSLOG("alc", "failed to create property array");
		OSSafeReleaseNULL(pathMapsDriverArray);
		OSSafeReleaseNULL(layoutsDriverArray);
		return;
	}
	
	OSArray *codecInfoArray = nullptr;
	if (getKernelVersion() == KernelVersion::Tiger) {
		codecInfoArray = OSArray::withCapacity((uint32_t) codecs.size());
		if (!codecInfoArray) {
			SYSLOG("alc", "failed to create CodecInfo array");
			OSSafeReleaseNULL(pathMapsDriverArray);
			OSSafeReleaseNULL(layoutsDriverArray);
			return;
		}
	}

	for (size_t i = 0, s = codecs.size(); i < s; i++) {
		DBGLOG("alc", "adding codec %X:%X:%X", codecs[i]->vendor, codecs[i]->codec, codecs[i]->revision);

		auto info = codecs[i]->info;
		if (!info) {
			SYSLOG("alc", "missing CodecModInfo for %lu codec at resource updating", i);
			continue;
		}
		
		size_t num = info->platformNum;
		DBGLOG("alc", "selecting platform from %lu files", num);
		for (size_t f = 0; f < num; f++) {
			auto &fi = info->platforms[f];
			DBGLOG("alc", "comparing %lu layout %X/%X", f, fi.layout, controllers[codecs[i]->controller]->layout);
			if (controllers[codecs[i]->controller]->layout == fi.layout && KernelPatcher::compatibleKernel(fi.minKernel, fi.maxKernel)) {
				DBGLOG("alc", "found platform at %lu index", f);
				
				auto dict = unserializeCodecDictionary(fi.data, fi.dataLength);
				if (!dict) {
					SYSLOG("alc", "failed to extract layout data");
					break;
				}

				auto pathMaps = dict->getObject("PathMaps");
				if (pathMaps) {
					auto pathMapsArray = OSDynamicCast(OSArray, pathMaps);
					if (pathMapsArray) {
						pathMapsDriverArray->merge(pathMapsArray);
					} else {
						SYSLOG("alc", "PathMaps element is not an array");
					}
				} else {
					SYSLOG("alc", "failed to get PathMaps element");
				}
				
				dict->release();
				break;
			}
		}
		
		num = info->layoutNum;
		DBGLOG("alc", "selecting layout from %lu files", num);
		for (size_t f = 0; f < num; f++) {
			auto &fi = info->layouts[f];
			DBGLOG("alc", "comparing %lu layout %X/%X", f, fi.layout, controllers[codecs[i]->controller]->layout);
			if (controllers[codecs[i]->controller]->layout == fi.layout && KernelPatcher::compatibleKernel(fi.minKernel, fi.maxKernel)) {
				DBGLOG("alc", "found layout at %lu index", f);
				
				auto dict = unserializeCodecDictionary(fi.data, fi.dataLength);
				if (!dict) {
					SYSLOG("alc", "failed to extract platform data");
					break;
				}

				// Replace layout ID if a different layout ID is being reported to the OS.
				if (layoutIdIsOverridden) {
					auto layoutNum = OSNumber::withNumber(layoutIdOverride, 32);
					if (layoutNum) {
						dict->setObject("LayoutID", layoutNum);
						layoutNum->release();
					} else {
						SYSLOG("alc", "failed to set LayoutID");
					}
				}

				layoutsDriverArray->setObject(dict);
				dict->release();
				break;
			}
		}
		
		// 10.4 requires a CodecInfo dictionary.
		if (getKernelVersion() == KernelVersion::Tiger) {
			auto codecInfo = OSDictionary::withCapacity(3);
			if (!codecInfo) {
				SYSLOG("alc", "failed to create codecinfo data");
				break;
			}
			
			auto softVolumeDict = OSDictionary::withCapacity(1);
			auto volumeDict = OSDictionary::withCapacity(1);
			auto signalProcessingDict = OSDictionary::withCapacity(1);
			auto analogOutDict = OSDictionary::withCapacity(1);
			auto codecId = OSNumber::withNumber(codecs[i]->codec | (codecs[i]->vendor << 16), 32);
			
			if (softVolumeDict && volumeDict && signalProcessingDict && analogOutDict && codecId) {
				volumeDict->setObject("SoftwareVolume", softVolumeDict);
				signalProcessingDict->setObject("Volume", volumeDict);
				analogOutDict->setObject("SignalProcessing", signalProcessingDict);
				codecInfo->setObject("AnalogOut", analogOutDict);
				codecInfo->setObject("CodecID", codecId);
				
				codecInfoArray->setObject(codecInfo);
			} else {
				SYSLOG("alc", "failed to create one or more codecinfo dictionaries");
			}

			OSSafeReleaseNULL(softVolumeDict);
			OSSafeReleaseNULL(volumeDict);
			OSSafeReleaseNULL(signalProcessingDict);
			OSSafeReleaseNULL(analogOutDict);
			OSSafeReleaseNULL(codecId);
			OSSafeReleaseNULL(codecInfo);
		}
	}
	
	// Replace existing layouts and pathmaps.
	service->setProperty("Layouts", layoutsDriverArray);
	service->setProperty("PathMaps", pathMapsDriverArray);
	
	if (getKernelVersion() == KernelVersion::Tiger) {
		service->setProperty("CodecInfo", codecInfoArray);
		codecInfoArray->release();
	}

	layoutsDriverArray->release();
	pathMapsDriverArray->release();
}

OSDictionary* AlcEnabler::unserializeCodecDictionary(const uint8_t *data, uint32_t dataLength) {
	OSString *errorString = nullptr;
	OSDictionary *parsedDict = nullptr;
	uint32_t bufferLength = 0x7A000; // Buffer size that AppleHDA uses.
	
	auto buffer = Compression::decompress(Compression::ModeZLIB, &bufferLength, data, dataLength, nullptr);
	if (!buffer) {
		return nullptr;
	}
	
	if (bufferLength != 0) {
		auto parsedXML = OSUnserializeXML((char*) buffer, &errorString);
		if (parsedXML) {
			parsedDict = OSDynamicCast(OSDictionary, parsedXML);
			if (!parsedDict) {
				parsedXML->release();
			}
		}
		
		if (!parsedDict) {
			const char *errorCString = "unknown error";
			if (errorString && errorString->getCStringNoCopy()) {
				errorCString = errorString->getCStringNoCopy();
			}
			
			SYSLOG("alc", "failed to unserialize XML: %s", errorCString);
		}
	} else {
		SYSLOG("alc", "failed to decompress zlib");
	}
	
	Buffer::deleter(buffer);
	return parsedDict;
}
#endif

bool AlcEnabler::validateInjection(IORegistryEntry *hdaService) {
	// Check for no-controller-inject. If set, ignore the controller.
	bool noControllerInject = nullptr != hdaService->getProperty("no-controller-inject");
	if (noControllerInject)
		SYSLOG("alc", "not injecting %s", safeString(hdaService->getName()));
	
	return !noControllerInject;
}

void AlcEnabler::applyPatches(KernelPatcher &patcher, size_t index, const KextPatch *patches, size_t patchNum) {
	for (size_t p = 0; p < patchNum; p++) {
		auto &patch = patches[p];
		if (patch.patch.kext->loadIndex == index) {
			DBGLOG("alc", "checking patch %lu for %lu kext (%s)", p, index, patch.patch.kext->id);
			if (patcher.compatibleKernel(patch.minKernel, patch.maxKernel)) {
				DBGLOG("alc", "applying patch %lu for %lu kext (%s)", p, index, patch.patch.kext->id);
				patcher.applyLookupPatch(&patch.patch);
				// Do not really care for the errors for now
				patcher.clearError();
			}
		}
	}
}
