//
//  ALCUserClientProvider.cpp
//  AppleALC
//
//  Created by Nick on 10/14/20.
//  Copyright © 2020 vit9696. All rights reserved.
//

#include "ALCUserClientProvider.hpp"

OSDefineMetaClassAndStructors(ALCUserClientProvider, IOService);

bool ALCUserClientProvider::start(IOService* provider) {
	if (!super::start(provider))
		return false;
	
	mHDACodecDevice = IOService::waitForMatchingService(IOService::nameMatching(kIOHDACodecDevice), 3000);	// Wait for 3s
	
	if (!mHDACodecDevice)
	{
		DBGLOG("client", "HDACodecDevice not found");
		stop(provider);
		return false;
	}
	
	// Only attach to internal codec
	auto codecAddress = OSDynamicCast(OSNumber, mHDACodecDevice->getProperty(kIOHDACodecAddress));
	if (!codecAddress) {
		DBGLOG("client", "IOHDACodec address is non-existent");
		stop(provider);
		return false;
	}
	
	// Check if attached to correct IOHDACodecDevice
	if (codecAddress->unsigned8BitValue() != 0) {
		DBGLOG("client", "IOHDACodec address has a non zero value");
		stop(provider);
		return false;
	}
	
	// We are ready for verbs
	SYSLOG("client", "ALCUserClient is ready for hda-verbs");
	mHDACodecDevice->setProperty("ReadyForALCVerbs", kOSBooleanTrue);
	readyForVerbs = true;
	
	// Publish the service
	registerService();
	
	return true;
}

void ALCUserClientProvider::stop(IOService* provider) {
	super::stop(provider);
	
	OSSafeReleaseNULL(mHDACodecDevice);
}

bool ALCUserClientProvider::init(OSDictionary* dictionary) {
	if (!super::init(dictionary)) {
		return false;
	}

	return true;
}

void ALCUserClientProvider::free() {
	super::free();
}

IOReturn ALCUserClientProvider::sendHdaCommand(uint16_t nid, uint16_t verb, uint16_t param) {
	auto sharedAlc = AlcEnabler::getShared();
	
	if (!sharedAlc) {
		DBGLOG("client", "Unable to get shared AlcEnabler instance");
		return kIOReturnError;
	}
	
	DBGLOG("client", "Send HDA command nid=0x%X, verb=0x%X, param=0x%X", nid, verb, param);
	UInt* ret { nullptr };
	return sharedAlc->IOHDACodecDevice_executeVerb((void*)mHDACodecDevice, nid, verb, param, ret, true);
}
