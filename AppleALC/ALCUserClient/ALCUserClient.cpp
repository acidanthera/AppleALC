//
//  ALCUserClient.cpp
//  AppleALC
//
//  Created by Nick on 10/14/20.
//  Copyright © 2020 vit9696. All rights reserved.
//

#include <IOKit/IOLib.h>

#include "ALCUserClient.hpp"

OSDefineMetaClassAndStructors(ALCUserClient, IOUserClient);

#if __MAC_OS_X_VERSION_MIN_REQUIRED > __MAC_10_4
const IOExternalMethodDispatch ALCUserClient::sMethods[kNumberOfMethods] = {
	{ //kMethodExecuteVerb
		reinterpret_cast<IOExternalMethodAction>(&ALCUserClient::methodExecuteVerb),	// Method pointer
		3,																				// Num of scalar input values
		0,																				// Num of struct input values
		1,																				// Num of scalar output values
		0																				// Num of struct output values
	}
};

IOReturn ALCUserClient::externalMethod(uint32_t selector, IOExternalMethodArguments* arguments, IOExternalMethodDispatch* dispatch, OSObject* target, void* reference) {
	if (selector >= kNumberOfMethods)
		return kIOReturnUnsupported;
	
	dispatch = const_cast<IOExternalMethodDispatch*>(&sMethods[selector]);
	
	target = mProvider;
	reference = NULL;
	
	return super::externalMethod(selector, arguments, dispatch, target, reference);
}

#else
IOExternalMethodACID ALCUserClient::sMethodsLegacy[kNumberOfMethods] = {
	{ //kMethodExecuteVerb
		NULL,
#if defined(__i386__)
		kIOExternalMethodACIDPadding,
#endif
		(IOMethodACID) sendHdaCommandInternal,
#if defined(__x86_64__)
		kIOExternalMethodACIDPadding,
#endif
		kIOUCScalarIScalarO,
		3,
		1
	}
};

IOExternalMethod* ALCUserClient::getTargetAndMethodForIndex(IOService **targetP, UInt32 index) {
	if (index >= kNumberOfMethods)
		return NULL;

	*targetP = this;
	return reinterpret_cast<IOExternalMethod*>(&sMethodsLegacy[index]);
}

IOReturn ALCUserClient::sendHdaCommandInternal(ALCUserClient *that, uint16_t nid, uint16_t verb, uint16_t param, uint64_t *outVal) {
	*outVal = that->mProvider->sendHdaCommand(nid, verb, param);
	return kIOReturnSuccess;
}
#endif

bool ALCUserClient::initWithTask(task_t owningTask, void* securityToken, UInt32 type, OSDictionary* properties) {
	if (!owningTask)
		return false;
	
	if (!super::initWithTask(owningTask, securityToken, type))
		return false;
	
	mTask = owningTask;
	mProvider = NULL;
	
	return true;
}

bool ALCUserClient::start(IOService* provider) {
	bool success;
	
	mProvider = OSDynamicCast(ALCUserClientProvider, provider);
	success = (mProvider != NULL);

	if (success)
		success = super::start(provider);
	
	return success;
}

IOReturn ALCUserClient::clientClose() {
	if (!isInactive())
		terminate();
	
	return kIOReturnSuccess;
}

IOReturn ALCUserClient::methodExecuteVerb(ALCUserClientProvider* target, void* ref, IOExternalMethodArguments* args) {
	uint16_t nid, verb, params;
	
	nid 	= static_cast<uint16_t>(args->scalarInput[0]);
	verb 	= static_cast<uint16_t>(args->scalarInput[1]);
	params 	= static_cast<uint16_t>(args->scalarInput[2]);
	
	args->scalarOutput[0] = target->sendHdaCommand(nid, verb, params);
	return kIOReturnSuccess;
}
