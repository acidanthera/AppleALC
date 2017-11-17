//
//  kern_resource.hpp
//  AppleALC
//
//  Copyright © 2016-2017 vit9696. All rights reserved.
//

#ifndef kern_resource_hpp
#define kern_resource_hpp

#include <Headers/kern_util.hpp>
#include <Headers/kern_patcher.hpp>
#include <Headers/kern_iokit.hpp>

#include <sys/types.h>
#include <stdint.h>

/**
 *  IORegistry path sequence for finding IOHDACodec info and layout number
 *  correspounds to CodecLookup.plist resource file
 */
struct CodecLookupInfo {
	const char **tree;
	size_t treeSize;
	size_t controllerNum;
	bool detect;
};

struct KextPatch {
	KernelPatcher::LookupPatch patch;
	uint32_t minKernel;
	uint32_t maxKernel;
};

/**
 *  Corresponds to a Controllers.plist entry
 */
struct ControllerModInfo {
	static constexpr uint32_t PlatformAny {0};
	const char *name;
	uint32_t vendor;
	uint32_t device;
	const uint32_t *revisions;
	size_t revisionNum;
	uint32_t platform;
	int computerModel;
	const KextPatch *patches;
	size_t patchNum;
};

/**
 *  Corresponds to Info.plist resource file of each codec
 */
struct CodecModInfo {
	struct File {
		const uint8_t *data;
		uint32_t dataLength;
		uint32_t minKernel;
		uint32_t maxKernel;
		uint32_t layout;
	};

	const char *name;
	uint16_t codec;
	const uint32_t *revisions;
	size_t revisionNum;
	
	const File *platforms;
	size_t platformNum;
	const File *layouts;
	size_t layoutNum;
	const KextPatch *patches;
	size_t patchNum;
};

/**
 *  Contains all the supported codecs by a specific vendor
 *  Corresponds to Vendors.plist resource file
 */
struct VendorModInfo {
	const char *name;
	uint16_t vendor;
	const CodecModInfo *codecs;
	const size_t codecsNum;
};

/**
 *  Generated resource data
 */
extern CodecLookupInfo ADDPR(codecLookup)[];
extern const size_t ADDPR(codecLookupSize);

extern KernelPatcher::KextInfo ADDPR(kextList)[];
extern const size_t ADDPR(kextListSize);

extern ControllerModInfo ADDPR(controllerMod)[];
extern const size_t ADDPR(controllerModSize);

extern VendorModInfo ADDPR(vendorMod)[];
extern const size_t ADDPR(vendorModSize);


#endif /* kern_resource_hpp */
