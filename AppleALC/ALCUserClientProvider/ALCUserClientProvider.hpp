//
//  ALCUserClientProvider.hpp
//  AppleALC
//
//  Created by Nick on 10/14/20.
//  Copyright © 2020 vit9696. All rights reserved.
//

#ifndef ALCUserClientProvider_hpp
#define ALCUserClientProvider_hpp

#define kIOHDACodecDevice	"IOHDACodecDevice"

#include<IOKit/IOService.h>
#include <IOKit/IOUserClient.h>

#include "kern_alc.hpp"

class EXPORT ALCUserClientProvider : public IOService {
	using super = IOService;
	OSDeclareDefaultStructors(ALCUserClientProvider);

	IOService* 	hdaCodecDevice { nullptr };
	bool		readyForVerbs	{ false	};

public:
	virtual IOService* probe(IOService *provider, SInt32 *score) override;
	virtual bool start(IOService* provider) override;
	virtual void stop(IOService* provider) override;
	
	/**
	 *  Called by user-client to set the codec verbs
	 *
	 *  @param nid Node ID
	 *  @param verb The hda-verb command to send (as defined in hdaverb.h)
	 *  @param param The parameters for the verb
	 *
	 *  @return kIOReturnSuccess on successful execution
	 */
	virtual uint64_t sendHdaCommand(uint16_t nid, uint16_t verb, uint16_t param);
};

#endif /* ALCUserClientProvider_hpp */
