//
//  kern_audio.hpp
//  AppleALC
//
//  Copyright © 2018 vit9696. All rights reserved.
//

#ifndef kern_audio_hpp
#define kern_audio_hpp

class EXPORT AppleALCAudio : public IOService {
	OSDeclareDefaultStructors(AppleALCAudio)
public:
	IOService *probe(IOService *provider, SInt32 *score) override;
};

#endif /* kern_audio_hpp */
