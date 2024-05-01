//
//  ALCUserClient.hpp
//  AppleALC
//
//  Created by Nick on 10/14/20.
//  Copyright © 2020 vit9696. All rights reserved.
//

#ifndef ALCUserClient_hpp
#define ALCUserClient_hpp

#include <IOKit/IOService.h>
#include <IOKit/IOUserClient.h>

#include "UserKernelShared.h"
#include "ALCUserClientProvider.hpp"

class EXPORT ALCUserClient : public IOUserClient {
	using super = IOUserClient;
	OSDeclareDefaultStructors(ALCUserClient);

	ALCUserClientProvider* mProvider { nullptr };
	task_t mTask {nullptr};

#if __MAC_OS_X_VERSION_MIN_REQUIRED > __MAC_10_4
	static const IOExternalMethodDispatch sMethods[kNumberOfMethods];
#else
	static IOExternalMethodACID sMethodsLegacy[kNumberOfMethods];
	static IOReturn sendHdaCommandInternal(ALCUserClient *that, uint16_t nid, uint16_t verb, uint16_t param, uint64_t *outVal);
#endif
	
public:
	virtual bool start(IOService* provider) APPLE_KEXT_OVERRIDE;
	
	virtual bool initWithTask(task_t owningTask, void* securityToken,
							  UInt32 type, OSDictionary* properties) APPLE_KEXT_OVERRIDE;

	virtual IOReturn clientClose() APPLE_KEXT_OVERRIDE;
#if __MAC_OS_X_VERSION_MIN_REQUIRED > __MAC_10_4
	virtual IOReturn externalMethod(uint32_t selector, IOExternalMethodArguments* arguments,
									IOExternalMethodDispatch* dispatch, OSObject* target,
									void* reference) APPLE_KEXT_OVERRIDE;
#else
	virtual IOExternalMethod *getTargetAndMethodForIndex(IOService **targetP, UInt32 index) APPLE_KEXT_OVERRIDE;
#endif
	
protected:
	static IOReturn methodExecuteVerb(ALCUserClientProvider* target, void* ref,
									  IOExternalMethodArguments* args);
};

#endif /* ALCUserClient_hpp */
