//
//  kern_alc.hpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#ifndef kern_alc_hpp
#define kern_alc_hpp

#include "kern_patcher.hpp"
#include "kern_resources.hpp"

class AlcEnabler {
public:
	bool init();
	void deinit();
	
private:
	KernelPatcher patcher;
	
	/**
	 *  Load kext files and prepare callbacks for processing
	 *
	 *  @return true on success
	 */
	bool loadKexts();
	
	/**
	 *  Patch AppleHDA or another kext if needed and prepare other patches
	 *
	 *  @param index   kinfo handle
	 *  @param address kinfo load address
	 *  @param size    kinfo memory size
	 */
	void processKext(size_t index, mach_vm_address_t address, size_t size);
	
	/**
	 *  ResourceLoad callback type
	 */
	using t_callback = void (*)(uint32_t, kern_return_t, const void *, uint32_t, void *);
	
	/**
	 *  Hooked ResourceLoad callbacks returning correct layout/platform
	 */
	static void layoutLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context);
	static void platformLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context);

	/**
	 *  Trampolines for original method invocations
	 */
	t_callback orgLayoutLoadCallback {nullptr};
	t_callback orgPlatformLoadCallback {nullptr};
	
	/**
	 *  Detects audio controllers
	 */
	void grabControllers();
	
	/**
	 *  Detects audio codecs
	 *
	 *  @return see validateCodecs
	 */
	bool grabCodecs();
	
	/**
	 *  Compare found controllers with built-in mod lists
	 *  Unlike validateCodecs() does not remove anything from
	 *  controllers but only sets their infos.
	 */
	void validateControllers();
	
	/**
	 *  Compare found codecs with built-in mod lists
	 *
	 *  @return true if anything suitable found
	 */
	bool validateCodecs();

	/**
	 *  Apply kext patches for loaded kext index
	 *
	 *  @param index      kinfo index
	 *  @param patches    patch list
	 *  @param patchesNum patch number
	 */
	void applyPatches(size_t index, const KextPatch *patches, size_t patchesNum);

	/**
	 *  Supported resource types
	 */
	enum class Resource {
		Layout,
		Platform
	};

	/**
	 *  Update resource request parameters with hooked data if necessary
	 *
	 *  @param type               resource type
	 *  @param result             kOSReturnSuccess on resource update
	 *  @param resourceData       resource data reference
	 *  @param resourceDataLength resource data length reference
	 */
	void updateResource(Resource type, kern_return_t &result, const void * &resourceData, uint32_t &resourceDataLength);

	/**
	 *  Controller identification and modification info
	 */
	class ControllerInfo {
		ControllerInfo(uint32_t ven, uint32_t dev, uint32_t rev, uint32_t p, uint32_t lid, bool d) :
		vendor(ven), device(dev), revision(rev), platform(p), layout(lid), detect(d) {}
	public:
		static ControllerInfo *create(uint32_t ven, uint32_t dev, uint32_t rev, uint32_t p, uint32_t lid, bool d) {
			return new ControllerInfo(ven, dev, rev, p, lid, d);
		}
		static void deleter(ControllerInfo *info) { delete info; }
		const ControllerModInfo *info {nullptr};
		const CodecLookupInfo *lookup {nullptr};
		uint32_t const vendor;
		uint32_t const device;
		uint32_t const revision;
		uint32_t const platform {ControllerModInfo::PlatformAny};
		uint32_t const layout;
		bool const detect;
	};
	
	/**
	 *  Detected controllers
	 */
	evector<ControllerInfo *, ControllerInfo::deleter> controllers;
	size_t currentController {0};

	/**
	 *  Codec identification and modification info
	 */
	class CodecInfo {
		CodecInfo(size_t ctrl, uint64_t ven, uint32_t rev) :
		controller(ctrl), revision(rev) {
			vendor = (ven & 0xFFFF0000) >> 16;
			codec = ven & 0xFFFF;
		}
	public:
		static CodecInfo *create(size_t ctrl, uint64_t ven, uint32_t rev) {
			return new CodecInfo(ctrl, ven, rev);
		}
		static void deleter(CodecInfo *info) { delete info; }
		const CodecModInfo *info {nullptr};
		size_t controller;
		uint16_t vendor;
		uint16_t codec;
		uint32_t revision;
	};
	
	/**
	 *  Detected and validated codec infos
	 */
	evector<CodecInfo *, CodecInfo::deleter> codecs;
	
	/**
	 *  Current progress mask
	 */
	struct ProcessingState {
		enum {
			NotReady = 0,
			ControllersLoaded = 1,
			CodecsLoaded = 2,
			CallbacksWantRouting = 4,
			CallbacksRouted = 8,
		};
	};
	int progressState;
	
	/**
	 *  Detected ComputerModel
	 */	int computerModel;

};

#endif /* kern_alc_hpp */
