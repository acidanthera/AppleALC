//
//  kern_iokit.cpp
//  AppleALC
//
//  Copyright © 2016 vit9696. All rights reserved.
//

#include "kern_iokit.hpp"
#include "kern_util.hpp"

#include <libkern/c++/OSSerialize.h>
#include <IOKit/IORegistryEntry.h>
#include <IOKit/IOService.h>
#include <IOKit/IODeviceTreeSupport.h>

namespace IOUtil {

	OSSerialize *getProperty(IORegistryEntry *entry, const char *property) {
		auto value = entry->getProperty(property);
		if (value) {
			auto s = OSSerialize::withCapacity(PAGE_SIZE);
			if (value->serialize(s)) {
				return s;
			} else {
				SYSLOG("ioutil @ failed to serialise %s property", property);
				s->release();
			}
		} else {
			DBGLOG("ioutil @ failed to get %s property", property);
		}
		return nullptr;
	}
	
	int getComputerModel() {
		auto entry = IORegistryEntry::fromPath("/", gIODTPlane);
		if (entry) {
			auto prop =  entry->getProperty("compatible");
			if (prop) {
				auto data = OSDynamicCast(OSData, prop);
				if (data) {
					//TODO: make this more reliable
					if (strstr(static_cast<const char *>(data->getBytesNoCopy()), "Book", strlen("Book"))) {
						return ComputerModel::ComputerLaptop;
					} else {
						return ComputerModel::ComputerDesktop;
					}
				} else {
					DBGLOG("ioutil @ compatible property has invalid format");
				}
			} else {
				DBGLOG("ioutil @ failed to get compatible property");
			}
		}
		DBGLOG("ioutil @ failed to get DT entry");
		return ComputerModel::ComputerAny;
	}
	
	IORegistryEntry *findEntryByPrefix(const char *path, const char *prefix, const IORegistryPlane *plane, bool (*proc)(IORegistryEntry *), bool brute) {
		auto entry = IORegistryEntry::fromPath(path, plane);
		if (entry) {
			auto res = findEntryByPrefix(entry, prefix, plane, proc, brute);
			entry->release();
			return res;
		}
		DBGLOG("ioutil @ failed to get %s entry", path);
		return nullptr;
	}
	

	IORegistryEntry *findEntryByPrefix(IORegistryEntry *entry, const char *prefix, const IORegistryPlane *plane, bool (*proc)(IORegistryEntry *), bool brute) {
		bool found {false};
		IORegistryEntry *res {nullptr};
		
		size_t bruteCount {0};
		
		do {
			bruteCount++;
			auto iterator = entry->getChildIterator(plane);
			
			if (iterator) {
				size_t len = strlen(prefix);
				while ((res = OSDynamicCast(IORegistryEntry, iterator->getNextObject())) != nullptr) {
					//DBGLOG("ioutil @ iterating over %s", res->getName());
					if (!strncmp(prefix, res->getName(), len)) {
						found = proc ? proc(res) : true;
						if (found) {
							if (bruteCount > 1)
								DBGLOG("ioutil @ bruted %s value in %zu attempts", prefix, bruteCount);
							if (!proc)
								break;
						}
					}
				}
				iterator->release();
				
			} else {
				SYSLOG("ioutil @ failed to iterate over entry");
				return nullptr;
			}
			
		} while (brute && bruteCount < bruteMax && !found);
		
		if (!found)
			DBGLOG("ioutil @ failed to find %s", prefix);
		return proc ? nullptr : res;
	}
}
