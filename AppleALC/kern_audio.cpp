//
//  kern_audio.cpp
//  AppleALC
//
//  Copyright © 2018 vit9696. All rights reserved.
//

#include <Headers/kern_iokit.hpp>
#include <Headers/plugin_start.hpp>

#include "kern_audio.hpp"

OSDefineMetaClassAndStructors(AppleALCAudio, IOService)

IOService *AppleALCAudio::probe(IOService *hdaService, SInt32 *score) {
	if (!ADDPR(startSuccess))
		return nullptr;

	if (!hdaService) {
		DBGLOG("audio", "received null digitial audio device");
		return nullptr;
	}

	uint32_t hdaVen, hdaDev;
	if (!WIOKit::getOSDataValue(hdaService, "vendor-id", hdaVen) ||
		!WIOKit::getOSDataValue(hdaService, "device-id", hdaDev)) {
		SYSLOG("audio", "found an unknown device");
		return nullptr;
	}

	auto hdaPlaneName = hdaService->getName();
	DBGLOG("audio", "corrects analog audio for hdef at %s with %04X:%04X",
		   safeString(hdaPlaneName), hdaVen, hdaDev);

	if (hdaVen != WIOKit::VendorID::Intel) {
		DBGLOG("audio", "unsupported hdef vendor");
		return nullptr;
	}

	// HDEF device is always named and is not named as B0D3 or HDAU.
	bool isAnalog = hdaPlaneName && strcmp(hdaPlaneName, "B0D3") && strcmp(hdaPlaneName, "HDAU");

	if (!isAnalog) {
		DBGLOG("audio", "found digital audio, ignoring.");
		return nullptr;
	}

	// AppleHDAController only recognises HDEF and HDAU.
	if (!hdaPlaneName || strcmp(hdaPlaneName, "HDEF")) {
		DBGLOG("audio", "fixing audio plane name to HDEF");
		WIOKit::renameDevice(hdaService, "HDEF");
	}

	uint32_t layout = 0;
	if (PE_parse_boot_argn("alcid", &layout, sizeof(layout))) {
		DBGLOG("audio", "found layout-id override %d", layout);
		hdaService->setProperty("layout-id", &layout, sizeof(layout));
	}

	if (!hdaService->getProperty("built-in")) {
		DBGLOG("audio", "fixing built-in in hdef");
		uint8_t builtBytes[] { 0x00 };
		hdaService->setProperty("built-in", builtBytes, sizeof(builtBytes));
	} else {
		DBGLOG("audio", "found existing built-in in hdef");
	}

	// These seem to fix AppleHDA warnings, perhaps research them later.
	// They are probably related to the current volume of the boot bell sound.
	if (!hdaService->getProperty("MaximumBootBeepVolume")) {
		DBGLOG("audio", "fixing MaximumBootBeepVolume in hdef");
		uint8_t bootBeepBytes[] { 0xEE };
		hdaService->setProperty("MaximumBootBeepVolume", bootBeepBytes, sizeof(bootBeepBytes));
	}

	if (!hdaService->getProperty("MaximumBootBeepVolumeAlt")) {
		DBGLOG("audio", "fixing MaximumBootBeepVolumeAlt in hdef");
		uint8_t bootBeepBytes[] { 0xEE };
		hdaService->setProperty("MaximumBootBeepVolumeAlt", bootBeepBytes, sizeof(bootBeepBytes));
	}

	if (!hdaService->getProperty("PinConfigurations")) {
		DBGLOG("audio", "fixing PinConfigurations in hdef");
		uint8_t pinBytes[] { 0x00 };
		hdaService->setProperty("PinConfigurations", pinBytes, sizeof(pinBytes));
	}

	return nullptr;
}

