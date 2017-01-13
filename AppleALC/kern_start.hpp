//
//  kern_start.hpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#ifndef kern_start_hpp
#define kern_start_hpp

#include "kern_alc.hpp"
#include "kern_policy.hpp"

#include <IOKit/IOService.h>

class PRODUCT_NAME : public IOService {
	OSDeclareDefaultStructors(PRODUCT_NAME)
public:
	bool init(OSDictionary *dict) override;
	bool start(IOService *provider) override;
	void stop(IOService *provider) override;
};

class Configuration {
	/**
	 *  Possible boot arguments
	 */
	static constexpr const char *bootargOff {"-alcoff"};        // Disable the kext
	static constexpr const char *bootargDebug {"-alcdbg"};      // Enable debug logging
	static constexpr const char *bootargLowMem {"-alclowmem"};  // Disable memory consuming operations
	static constexpr const char *bootargBeta {"-alcbeta"};      // Enforce on unsupported systems
	static constexpr const char *bootargPolicy {"-alcpolicy"};  // Use TrustedBSD policy
	static constexpr const char *bootargIOKit {"-alciokit"};    // Use IOKit::start method
	
	/**
	 *  Set once the arguments are parsed
	 */
	bool readArguments {false};
	
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
	mac_policy_ops policyOps {
		.mpo_policy_initbsd					= Policy::dummyPolicyInitBSD,
		.mpo_mount_check_remount			= policyCheckRemount
	};

public:
	/**
	 *  Retrieve boot arguments
	 *
	 *  @return true if allowed to continue
	 */
	bool getBootArguments();
	
	/**
	 *  Disable the extension by default
	 */
	bool isDisabled {true};

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
	 *  Audio enabler
	 */
	AlcEnabler enabler;
	
	/**
	 *  Policy controller
	 */
	Policy policy;

#ifdef DEBUG
	static constexpr const char *fullName {xStringify(PRODUCT_NAME) " Kernel Extension " xStringify(MODULE_VERSION) " DEBUG build"};
#else
	static constexpr const char *fullName {xStringify(PRODUCT_NAME) " Kernel Extension " xStringify(MODULE_VERSION)};
#endif
	
	Configuration() : policy(xStringify(PRODUCT_NAME), fullName, &policyOps) {}
};

#endif /* kern_start_hpp */
