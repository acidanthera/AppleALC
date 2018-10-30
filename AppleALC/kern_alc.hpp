//
//  kern_alc.hpp
//  AppleALC
//
//  Copyright © 2016-2017 vit9696. All rights reserved.
//

#ifndef kern_alc_hpp
#define kern_alc_hpp

#include <Headers/kern_patcher.hpp>
#include <Headers/kern_devinfo.hpp>

#include "kern_resources.hpp"

class AlcEnabler {
public:
	void init();
	void deinit();

private:
	/**
	 *  Update device properties for digital and analog audio support
	 */
	void updateProperties();

	/**
	 *  Update audio device properties
	 *
	 *  hdaService  audio device
	 *  info        device info
	 *  hdaGfx      hda-gfx property string or null
	 *  isAnalog    digital or analog audio device
	 */
	void updateDeviceProperties(IORegistryEntry *hdaService, DeviceInfo *info, const char *hdaGfx, bool isAnalog);

	/**
	 *  Maximum available connector count assumed on NVIDIA GPUs
	 */
	static constexpr size_t MaxConnectorCount = 6;

	/**
	 *  Remove log spam from AppleHDAController and AppleHDA
	 *
	 *  @param patcher KernelPatcher instance
	 *  @oaram index  kinfo handle
	 */
	void eraseRedundantLogs(KernelPatcher &patcher, size_t index);

	/**
	 *  Patch AppleHDA or another kext if needed and prepare other patches
	 *
	 *  @param patcher KernelPatcher instance
	 *  @param index   kinfo handle
	 *  @param address kinfo load address
	 *  @param size    kinfo memory size
	 */
	void processKext(KernelPatcher &patcher, size_t index, mach_vm_address_t address, size_t size);

	/**
	 *  Hooked ResourceLoad callbacks returning correct layout/platform
	 */
	static void layoutLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context);
	static void platformLoadCallback(uint32_t requestTag, kern_return_t result, const void *resourceData, uint32_t resourceDataLength, void *context);

	/**
	 *  Hooked AppleGFXHDA probe
	 */
	static IOService *gfxProbe(IOService *ctrl, IOService *provider, SInt32 *score);

	/**
	 *  Trampolines for original method invocations
	 */
	mach_vm_address_t orgLayoutLoadCallback {0};
	mach_vm_address_t orgPlatformLoadCallback {0};
	mach_vm_address_t orgGfxProbe {0};

	/**
	 *  @enum IOAudioDevicePowerState
	 *  @abstract Identifies the power state of the audio device
	 *  @discussion A newly created IOAudioDevices defaults to the idle state.
	 *  @constant kIOAudioDeviceSleep State set when the system is going to sleep
	 *  @constant kIOAudioDeviceIdle State when the system is awake but none of the IOAudioEngines are in use
	 *  @constant kIOAudioDeviceActive State when one ore more IOAudioEngines are in use.  This state transition must complete before the system will begin playing audio.
	 */
	enum ALCAudioDevicePowerState {
		ALCAudioDeviceSleep 	= 0,	// When sleeping
		ALCAudioDeviceIdle		= 1,	// When no audio engines running
		ALCAudioDeviceActive 	= 2		// audio engines running
	};

	/**
	 *  Obtain HDEF and HDAU layout-id
	 *
	 *  @param hdaDriver  ioreg driver instance
	 *
	 *  @return layout-id on successful find or 0
	 */
	static uint32_t getAudioLayout(IOService *hdaDriver);

	/**
	 *  Hooked performPowerChange method triggering a verb sequence on wake
	 */
	static IOReturn performPowerChange(IOService *hdaDriver, uint32_t from, uint32_t to, unsigned int *timer);

	/**
	 *  Hooked initializePinConfig method to preserve AppleHDACodecGeneric instance
	 */
	static IOReturn initializePinConfig(IOService *hdaCodec, IOService *configDevice);

	/**
	 *  AppleHDADriver::performPowerStateChange original method
	 */
	mach_vm_address_t orgPerformPowerChange {0};

	/**
	 *  AppleHDACodecGeneric::initializePinConfigDefaultFromOverride original method
	 */
	mach_vm_address_t orgInitializePinConfig {0};

	/**
	 *  Hooked entitlement copying method
	 */
	static void handleAudioClientEntitlement(task_t task, const char *entitlement, OSObject *&original);

	/**
	 *  Detects audio controllers
	 */
	void grabControllers();

	/**
	 *  Appends registered codec
	 *
	 *  @param user  AlcEnabler instance
	 *  @param e     found codec
	 *
	 *  @return true on success and false to continue bruting
	 */
	static bool appendCodec(void *user, IORegistryEntry *e);

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
	 *  @param patcher    KernelPatcher instance
	 *  @param index      kinfo index
	 *  @param patches    patch list
	 *  @param patchesNum patch number
	 */
	void applyPatches(KernelPatcher &patcher, size_t index, const KextPatch *patches, size_t patchesNum);

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
		ControllerInfo(uint32_t ven, uint32_t dev, uint32_t rev, uint32_t p, uint32_t lid, bool d, bool np) :
		vendor(ven), device(dev), revision(rev), platform(p), layout(lid), detect(d), nopatch(np) {}
	public:
		static ControllerInfo *create(uint32_t ven, uint32_t dev, uint32_t rev, uint32_t p, uint32_t lid, bool d, bool np) {
			return new ControllerInfo(ven, dev, rev, p, lid, d, np);
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
		bool const nopatch;
	};

	/**
	 *  Detected controllers
	 */
	evector<ControllerInfo *, ControllerInfo::deleter> controllers;
	size_t currentController {0};

	/**
	 *  Insert a controller with given parameters
	 */
	void insertController(uint32_t ven, uint32_t dev, uint32_t rev, bool np, uint32_t p=ControllerModInfo::PlatformAny, uint32_t lid=0, bool d=false, const CodecLookupInfo *lookup = nullptr) {
		auto controller = ControllerInfo::create(ven, dev, rev, p, lid, d, np);
		if (controller) {
			if (controllers.push_back(controller)) {
				controller->lookup = lookup;
			} else {
				SYSLOG("alc", "failed to store controller info for %X:%X:%X", ven, dev, rev);
				ControllerInfo::deleter(controller);
			}
		} else {
			SYSLOG("alc", "failed to create controller info for %X:%X:%X", ven, dev, rev);
		}
	}

	/**
	 *  Codec identification and modification info
	 */
	class CodecInfo {
		CodecInfo(size_t ctrl, uint32_t ven, uint32_t rev) :
		controller(ctrl), revision(rev) {
			vendor = (ven & 0xFFFF0000) >> 16;
			codec = ven & 0xFFFF;
		}
	public:
		static CodecInfo *create(size_t ctrl, uint32_t ven, uint32_t rev) {
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
			CallbacksWantRouting = 4
		};
	};
	int progressState {ProcessingState::NotReady};
	
	/**
	 *  Detected ComputerModel
	 */
	int computerModel {WIOKit::ComputerModel::ComputerInvalid};

	/**
	 *  HDAConfigDefault availability in AppleALC
	 */
	enum class WakeVerbMode {
		Detect,
		Enable,
		Disable
	};

	/**
	 *  Total available NVIDIA HDAU device-ids in 10.13 and newer
	 */
	static constexpr size_t MaxNvidiaDeviceIds = 16;

	/**
	 *  Magic NVIDIA HDAU id find to update the one from the list below
	 */
	static constexpr uint32_t NvidiaSpecialFind = 0x4144564E; // NVDA

	/**
	 *  NVIDIA HDAU device-ids available for replacement
	 */
	uint32_t nvidiaDeviceIdList[MaxNvidiaDeviceIds] {
		0x0E0A10DE, // GK104
		0x0E0B10DE, // GK106
		0x0E1B10DE, // GK107
		0x0E1A10DE, // GK110
		0x0BE510DE, // GF100
		0x0BEB10DE, // GF104
		0x0BE910DE, // GF106
		0x0BEA10DE, // GF108
		0x0E0910DE, // GF110
		0x0BEE10DE, // GF116
		0x0E0810DE, // GF119
		0x0BE310DE, // GF210
		0x0AC010DE, // MCP79
		0x0D9410DE, // MCP89
		0x0BE210DE, // GT216
		0x0BE410DE, // GTS 250M
	};

	/**
	 *  NVIDIA HDAU device-ids usage status
	 */
	bool nvidiaDeviceIdUsage[MaxNvidiaDeviceIds] = {};

	/**
	 *  Current NVIDIA device-id patch to use
	 */
	size_t currentFreeNvidiaDeviceId = 0;
};

#endif /* kern_alc_hpp */
