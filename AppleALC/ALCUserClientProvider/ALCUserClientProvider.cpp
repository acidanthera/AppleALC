//
//  ALCUserClientProvider.cpp
//  AppleALC
//
//  Created by Nick on 10/14/20.
//  Copyright © 2020 vit9696. All rights reserved.
//

#include "ALCUserClientProvider.hpp"
#include <Headers/kern_iokit.hpp>

OSDefineMetaClassAndStructors(ALCUserClientProvider, IOService);

IOService *ALCUserClientProvider::probe(IOService *provider, SInt32 *score) {
	hdaCodecDevice = provider;

	auto hdaController = hdaCodecDevice->getParentEntry(gIOServicePlane);
	if (!hdaController) {
		DBGLOG("client", "codec is missing AppleHDAController");
		return nullptr;
	}

	auto hdefDevice = hdaController->getParentEntry(gIOServicePlane);
	if (!hdefDevice) {
		DBGLOG("client", "controller is missing HDEF device");
		return nullptr;
	}

	uint32_t enableHdaVerbs = 0;
	WIOKit::getOSDataValue(hdefDevice, "alc-verbs", enableHdaVerbs);
	PE_parse_boot_argn("alcverbs", &enableHdaVerbs, sizeof(enableHdaVerbs));
	DBGLOG("client", "device %s to send custom verbs", enableHdaVerbs != 0 ? "allows" : "disallows");
	if (enableHdaVerbs == 0) {
		return nullptr;
	}

	return this;
}

bool ALCUserClientProvider::start(IOService* provider) {
	if (!super::start(provider))
		return false;

	// We are ready for verbs
	DBGLOG("client", "ALCUserClient is ready for hda-verbs");
	setProperty("ReadyForALCVerbs", kOSBooleanTrue);
	readyForVerbs = true;
	
	// Publish the service
	registerService();

	return true;
}

void ALCUserClientProvider::stop(IOService* provider) {
	super::stop(provider);
}

uint64_t ALCUserClientProvider::sendHdaCommand(uint16_t nid, uint16_t verb, uint16_t param) {
	if (!readyForVerbs) {
		DBGLOG("client", "provider not ready to accept hda-verb commands");
		return kIOReturnError;
	}
	
	auto sharedAlc = AlcEnabler::getShared();
	
	if (!sharedAlc) {
		DBGLOG("client", "unable to get shared AlcEnabler instance");
		return kIOReturnError;
	}

	if (!sharedAlc->orgIOHDACodecDevice_executeVerb) {
		DBGLOG("client", "unable to get verb support");
		return kIOReturnError;
	}
	
	unsigned ret = 0;
	sharedAlc->IOHDACodecDevice_executeVerb(reinterpret_cast<void*>(hdaCodecDevice), nid, verb, param, &ret, true);
	DBGLOG("client", "send HDA command nid=0x%X, verb=0x%X, param=0x%X, result=0x%08x", nid, verb, param, ret);
	
	return ret;
}
