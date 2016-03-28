//
//  kern_patcher.hpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#ifndef kern_patcher_hpp
#define kern_patcher_hpp

#include <mach/mach_types.h>

#include "kern_util.hpp"
#include "kern_mach.hpp"
#include "kern_disasm.hpp"

namespace Patch { union All; void deleter(All *); }
class OSKextLoadedKextSummaryHeader;

class KernelPatcher {
public:

	/**
	 *  Errors set by functions
	 */
	enum class Error {
		NoError,
		NoKinfoFound,
		NoSymbolFound,
		KernInitFailure,
		KernRunningInitFailure,
		KextListeningFailure,
		DisasmFailure,
		MemoryIssue,
		MemoryProtection,
		PointerRange
	};
	
	/**
	 *  Get last error
	 *
	 *  @return error code
	 */
	Error getError();
	
	/**
	 *  Reset all the previous errors
	 */
	void clearError();


	/**
	 *  Initialise KernelPatcher, prepare for modifications
	 */
	void init();
	
	/**
	 *  Deinitialise KernelPatcher, must be called regardless of the init error
	 */
	void deinit();

	/**
	 *  Kext information
	 */
	struct KextInfo {
		static constexpr size_t Unloaded {0};
		const char *id;
		const char **paths;
		size_t pathNum;
		bool detectCodecs;
		size_t loadIndex; // Updated after loading
	};

	/**
	 *  Loads and stores kinfo information locally
	 *
	 *  @param id       kernel item identifier
	 *  @param paths    item filesystem path array
	 *  @param num      number of path entries
	 *  @param isKernel kinfo is kernel info
	 *
	 *  @return loaded kinfo id
	 */
	size_t loadKinfo(const char *id, const char * const paths[], size_t num=1, bool isKernel=false);
	
	/**
	 *  Loads and stores kinfo information locally
	 *
	 *  @param info kext to load, updated on success
	 *
	 *  @return loaded kinfo id
	 */
	size_t loadKinfo(KextInfo *info);
	
	/**
	 *  Kernel kinfo id
	 */
	static constexpr size_t KernelID {0};
	
	/**
	 *  Update running information
	 *
	 *  @param id    loaded kinfo id
	 *  @param slide loaded slide
	 *  @param size  loaded memory size
	 */
	void updateRunningInfo(size_t id, mach_vm_address_t slide=0, size_t size=0);
	
	/**
	 *  Any kernel
	 */
	static constexpr uint32_t KernelAny {0};
	
	/**
	 *  Check kernel compatibility
	 *
	 *  @param min minimal requested version or KernelAny
	 *  @param max maximum supported version or KernelAny
	 *
	 *  @return true on success
	 */
	bool compatibleKernel(uint32_t min, uint32_t max);
	
	/**
	 *  Solve a kinfo symbol
	 *
	 *  @param id      loaded kinfo id
	 *  @param symbol  symbol to solve
	 *
	 *  @return running symbol address or 0
	 */
	mach_vm_address_t solveSymbol(size_t id, const char *symbol);
	
	/**
	 *  Hook kext loading and unloading to access kexts at early stage
	 */
	void setupKextListening();
	
	/**
	 *  Load handling structure
	 */
	class KextHandler {
		using t_handler = void (*)(KextHandler *);
		KextHandler(const char * const i, size_t idx, t_handler h) :
			id(i), index(idx), handler(h) {}
	public:
		static KextHandler *create(const char * const i, size_t idx, t_handler h) {
			return new KextHandler(i, idx, h);
		}
		static void deleter(KextHandler *i) {
			delete i;
		}
		
		void *self {nullptr};
		const char * const id {nullptr};
		size_t index {0};
		mach_vm_address_t address {0};
		size_t size {0};
		t_handler handler {nullptr};
	};
	
	/**
	 *  Enqueue handler processing at kext loading
	 *
	 *  @param handler  handler to process
	 */
	void waitOnKext(KextHandler *handler);

	/**
	 *  Arbitrary kext find/replace patch
	 */
	struct LookupPatch {
		KextInfo *kext;
		const uint8_t *find;
		const uint8_t *replace;
		size_t size;
		size_t count;
	};
	
	/**
	 *  Apply a find/replace patch
	 *
	 *  @param patch patch to apply
	 */
	void applyLookupPatch(const LookupPatch *patch);
	
	/**
	 *  Route function to function
	 *
	 *  @param from         function to route
	 *  @param to           routed function
	 *  @param buildWrapper create entrance wrapper
	 *  @param kernelRoute  kernel change requiring memory protection changes and patch reverting at unload
	 *
	 *  @return wrapper pointer or 0 on success
	 */
	mach_vm_address_t routeFunction(mach_vm_address_t from, mach_vm_address_t to, bool buildWrapper=false, bool kernelRoute=true);

private:

	/**
	 *  Created routed trampoline page
	 *
	 *  @param func original area
	 *  @param min  minimal amount of bytes that will be overwritten
	 *
	 *  @return trampoline pointer or 0
	 */
	mach_vm_address_t createTrampoline(mach_vm_address_t func, size_t min);
	
	/**
	 *  Called at kext loading and unloading if kext listening is enabled
	 */
	static void onKextSummariesUpdated();
	
	/**
	 *  A pointer to loaded kext information
	 */
	OSKextLoadedKextSummaryHeader **loadedKextSummaries {nullptr};

	/**
	 *  Local disassmebler instance, initialised on demand
	 */
	Disassembler disasm;

	/**
	 *  Loaded kernel items
	 */
	evector<MachInfo *, MachInfo::deleter> kinfos;
	
	/**
	 *  Applied patches
	 */
	evector<Patch::All *, Patch::deleter> kpatches;
	
	/**
	 *  Awaiting kext notificators
	 */
	evector<KextHandler *, KextHandler::deleter> khandlers;
	
	/**
	 *  Allocated pages
	 */
	evector<Page *, Page::deleter> kpages;
	
	/**
	 *  Current error code
	 */
	Error code {Error::NoError};
	static constexpr size_t INVALID {0};
	
	/**
	 *  Jump instruction sizes
	 */
	static constexpr size_t SmallJump {1 + sizeof(int32_t)};
	static constexpr size_t LongJump {2 * sizeof(uint64_t)};
	
	/**
	 *  Possible kernel paths
	 */
	static constexpr size_t kernelPathsNum {6};
	const char *kernelPaths[kernelPathsNum] {
		"/mach_kernel",
		"/System/Library/Kernels/kernel",	//since 10.10
		"/System/Library/Kernels/kernel.debug",
		"/System/Library/Kernels/kernel.development",
		"/System/Library/Caches/com.apple.kext.caches/Startup/kernelcache", //compressed one
		"/System/Library/PrelinkedKernels/prelinkedkernel" //compressed one
		
	};
};

#endif /* kern_patcher_hpp */
