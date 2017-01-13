//
//  kern_start.cpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#include "kern_start.hpp"
#include "kern_iokit.hpp"
#include "kern_alc.hpp"
#include "kern_patcher.hpp"
#include "kern_util.hpp"

#include <IOKit/IOLib.h>
#include <IOKit/IORegistryEntry.h>
#include <mach/mach_types.h>

OSDefineMetaClassAndStructors(PRODUCT_NAME, IOService)
Configuration config;

int Configuration::policyCheckRemount(kauth_cred_t cred, mount *mp, label *mlabel) {
	static bool initialised {false};
	
	DBGLOG("config @ policy hit");

	if (!initialised) {
		DBGLOG("config @ initialising enabler");
		initialised = config.enabler.init();
		if (!initialised) {
			DBGLOG("config @ initialisation failed");
			config.enabler.deinit();
		}
	}
	
	return 0;
}

bool Configuration::getBootArguments() {
	if (readArguments) return !isDisabled;
	
	isDisabled = false;
	char buf[16];
	
	bool compatible = KernelPatcher::compatibleKernel(KernelVersion::MountainLion, KernelVersion::Sierra);
	if (!compatible) {
		if (PE_parse_boot_argn(bootargBeta, buf, sizeof(buf))) {
			SYSLOG("init @ force enabling on an unsupported operating system");
		} else {
			SYSLOG("init @ automatically disabling on an unsupported operating system");
			isDisabled = true;
		}
	}

	isDisabled |= PE_parse_boot_argn(bootargOff, buf, sizeof(buf));
	isDisabled |= PE_parse_boot_argn("-s", buf, sizeof(buf));
	isDisabled |= PE_parse_boot_argn("-x", buf, sizeof(buf));
	
	debugEnabled = PE_parse_boot_argn(bootargDebug, buf, sizeof(buf));
	
	lowMemory = PE_parse_boot_argn(bootargLowMem, buf, sizeof(buf));
	
	if (PE_parse_boot_argn(bootargPolicy, buf, sizeof(buf))) {
		mode = StartMode::Policy;
	} else if (PE_parse_boot_argn(bootargIOKit, buf, sizeof(buf))) {
		mode = StartMode::IOKit;
	}
	
	readArguments = true;
	
	DBGLOG("config @ boot arguments disabled %d, debug %d", isDisabled, debugEnabled);
	
	if (isDisabled) {
		SYSLOG("init @ found a disabling argument or no arguments, exiting");
	}
	
	return !isDisabled;
}

bool PRODUCT_NAME::init(OSDictionary *dict) {
	if (!IOService::init(dict)) {
		SYSLOG("init @ failed to initalise the parent");
		return false;
	}
	
	return config.getBootArguments();
}

bool PRODUCT_NAME::start(IOService *provider) {
	if (!IOService::start(provider)) {
		SYSLOG("init @ failed to start the parent");
		return false;
	}
	
	if (config.mode == Configuration::StartMode::IOKit) {
		DBGLOG("init @ initialising with IOKit mode");
		
		bool res = config.enabler.init();
		if (!res) config.enabler.deinit();
		return res;
	} else {
		return true;
	}
}

void PRODUCT_NAME::stop(IOService *provider) {
	if (config.mode == Configuration::StartMode::IOKit) {
		config.enabler.deinit();
	}
	IOService::stop(provider);
	
	DBGLOG("init @ stopped");
}

extern "C" kern_return_t kern_start(kmod_info_t * ki, void *d) {
	DBGLOG("init @ start arrived");
	
	if (config.getBootArguments()) {
		if (config.mode == Configuration::StartMode::Policy) {
			DBGLOG("init @ initialising with Policy mode");
			
			if (config.policy.registerPolicy()) {
				return KERN_SUCCESS;
			}

			SYSLOG("init @ failed to register the policy");
		} else {
			return KERN_SUCCESS;
		}
	}
	
	return KERN_FAILURE;
}

extern "C" kern_return_t kern_stop(kmod_info_t *ki, void *d) {
	if (config.mode == Configuration::StartMode::Policy) {
		if (!config.policy.unregisterPolicy()) {
			SYSLOG("init @ cannot unregister the policy");
			return KERN_FAILURE;
		}
		config.enabler.deinit();
	}
	
	return KERN_SUCCESS;
}

