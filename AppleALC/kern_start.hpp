//
//  kern_start.hpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#ifndef kern_start_hpp
#define kern_start_hpp

#include "kern_alc.hpp"

#include <IOKit/IOService.h>
extern "C" {
	#include <security/mac_framework.h>
	#include <security/mac_policy.h>
}

class AppleALC : public IOService {
	OSDeclareDefaultStructors(AppleALC)
	
	/**
	 *  Possible boot arguments
	 */
	static constexpr const char *bootargOff {"-alcoff"};        // Disable the kext
	static constexpr const char *bootargDebug {"-alcdbg"};      // Enable debug logging
	static constexpr const char *bootargLowMem {"-alclowmem"};  // Disable memory consuming operations
	static constexpr const char *bootargPolicy {"-alcpolicy"};  // Use TrustedBSD policy
	static constexpr const char *bootargIOKit {"-alciokit"};    // Use IOKit::start method
	
	/**
	 *  Retrieve boot arguments
	 */
	void getBootArguments();
	
	/**
	 *  Disable the extension by default
	 */
	bool isDisabled {true};
	
	/**
	 *  Audio enabler
	 */
	static AlcEnabler enabler;
	
	/**
	 *  Enabler start variants
	 */
	enum class StartMode {
		IOKit,
		Policy
	};
	
	/**
	 *  Enabler start mode
	 */
	StartMode mode {StartMode::Policy};
	
	/**
	 *  TrustedBSD Policy handle
	 */
	mac_policy_handle_t policyHandle {0};
	
	/**
	 *  Called at TrustedBSD policy initialisation
	 *
	 *  @param conf policy configuration
	 */
	static void policyInitBSD(mac_policy_conf *conf);
	
	/**
	 *  TrustedBSD policy called before remounting
	 *  We use it to start AlcEnabler (at devfs remount)
	 *
	 *  @param cred     auth credential
	 *  @param mp       mount point
	 *  @param mlabel    mount point label
	 */
	static int policyCheckRemount(kauth_cred_t cred, mount *mp, label *mlabel);
	
	/**
	 *  TrustedBSD policy options
	 */
	static mac_policy_ops policyOps;
	
	/**
	 *  TrustedBSD policy configuration
	 */
	mac_policy_conf policyConf {
		.mpc_name				= "AppleALC",
		.mpc_fullname			= "AppleALC Kernel Module",
		.mpc_labelnames			= nullptr,
		.mpc_labelname_count	= 0,
		.mpc_ops				= &policyOps,
		.mpc_loadtime_flags		= MPC_LOADTIME_FLAG_UNLOADOK,
		.mpc_field_off			= nullptr,
		.mpc_runtime_flags		= 0
	};
public:
	bool init(OSDictionary *dict) override;
	bool start(IOService *provider) override;
	void stop(IOService *provider) override;
};

#endif /* kern_start_hpp */