/*
 *  Released under "The GNU General Public License (GPL-2.0)"
 *
 *  This program is free software; you can redistribute it and/or modify it
 *  under the terms of the GNU General Public License as published by the
 *  Free Software Foundation; either version 2 of the License, or (at your
 *  option) any later version.
 *
 *  This program is distributed in the hope that it will be useful, but
 *  WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 *  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <IOKit/IOKitLib.h>

#include <CoreFoundation/CoreFoundation.h>

#include "UserKernelShared.h"
#include "hdaverb.h"

static int compare_path(const void *a, const void *b)
{
	return strcmp(a, b);
}

static io_string_t *find_services(size_t *count)
{
	CFMutableDictionaryRef dict = IOServiceMatching(kALCUserClientProvider);

	io_iterator_t iterator;
	io_string_t *names = NULL;
	size_t nameCount = 0;
	kern_return_t kr = IOServiceGetMatchingServices(kIOMasterPortDefault, dict, &iterator);
	if (kr != KERN_SUCCESS)
	{
		fprintf(stderr, "Failed to iterate over ALC services: %08x.\n", kr);
		return NULL;
	}

	io_service_t service;
	while ((service = IOIteratorNext(iterator)) != 0) {
		io_string_t *newNames = realloc(names, (nameCount+1) * sizeof(names[0]));
		if (newNames == NULL)
		{
			fprintf(stderr, "Failed to allocate memory.\n");
			free(names);
			IOObjectRelease(iterator);
			return NULL;
		}

		kr = IORegistryEntryGetPath(service, kIOServicePlane, newNames[nameCount]);

		names = newNames;
		nameCount++;

		if(kr != kIOReturnSuccess)
		{
			fprintf(stderr, "Failed to obtain ALC service path: %08x.\n", kr);
			free(names);
			IOObjectRelease(iterator);
			return NULL;
		}
	}

	IOObjectRelease(iterator);

	if (nameCount == 0)
	{
		fprintf(stderr, "Failed to find ALCUserClientProvider services.\n");
		free(names);
		return NULL;
	}

	qsort(names, nameCount, sizeof(names[0]), compare_path);

	*count = nameCount;
	return names;
}

static io_service_t get_service(const char *name)
{
	// FIXME: Quite sure Apple API provides a better solution.
	CFMutableDictionaryRef dict = IOServiceMatching(kALCUserClientProvider);

	io_iterator_t iterator;
	kern_return_t kr = IOServiceGetMatchingServices(kIOMasterPortDefault, dict, &iterator);
	if (kr != KERN_SUCCESS)
	{
		fprintf(stderr, "Failed to iterate over ALC services: %08x.\n", kr);
		return 0;
	}

	io_service_t service;
	while ((service = IOIteratorNext(iterator)) != 0) {
		io_string_t foundName;
		kr = IORegistryEntryGetPath(service, kIOServicePlane, foundName);
		if(kr != kIOReturnSuccess)
		{
			fprintf(stderr, "Failed to obtain ALC service path: %08x.\n", kr);
			IOObjectRelease(iterator);
			return 0;
		}

		if (strcmp(name, foundName) == 0)
		{
			IOObjectRelease(iterator);
			return service;
		}
	}

	IOObjectRelease(iterator);
	fprintf(stderr, "Failed to find ALCUserClientProvider service %s.\n", name);
	return 0;
}

static unsigned execute_command(unsigned dev, uint16_t nid, uint16_t verb, uint16_t param)
{
	size_t nameCount = 0;
	io_string_t *names = find_services(&nameCount);

	if (names == NULL)
	{
		return kIOReturnError;
	}

	if (nameCount <= dev)
	{
		fprintf(stderr, "Failed to open ALCUserClientProvider service with specified id %u.\n", dev);
		free(names);
		return kIOReturnBadArgument;
	}

	io_service_t service = get_service(names[dev]);
	free(names);

	io_connect_t dataPort;
	kern_return_t kr = IOServiceOpen(service, mach_task_self(), 0, &dataPort);
	if (kr != kIOReturnSuccess)
	{
		fprintf(stderr, "Failed to open ALCUserClientProvider service: %08x.\n", kr);
		return kIOReturnError;
	}

	uint32_t inputCount = 3;	// Must match the declaration in ALCUserClient::sMethods
	uint64_t input[inputCount];
	input[0]	= nid;
	input[1]	= verb;
	input[2]	= param;
	
	uint64_t output;
	uint32_t outputCount = 1;

	kr = IOConnectCallScalarMethod(dataPort, kMethodExecuteVerb, input, inputCount, &output, &outputCount);
	
	if (kr != kIOReturnSuccess)
		return -1;
	
	return (unsigned)output;
}

static void list_keys(struct strtbl *tbl, int one_per_line)
{
	int c = 0;
	
	for (; tbl->str; tbl++)
	{
		unsigned long len = strlen(tbl->str) + 2;
		
		if (!one_per_line && c + len >= 80)
		{
			printf("\n");
			c = 0;
		}
		
		if (one_per_line)
			printf("  %s\n", tbl->str);
		else if (!c)
			printf("  %s", tbl->str);
		else
			printf(", %s", tbl->str);
		
		c += 2 + len;
	}
	
	if (!one_per_line)
		printf("\n");
}

/* look up a value from the given string table */
static int lookup_str(struct strtbl *tbl, const char *str)
{
	struct strtbl *p, *found;
	unsigned long len = strlen(str);
	
	found = NULL;
	
	for (p = tbl; p->str; p++)
	{
		if (!strncmp(str, p->str, len))
		{
			if (found)
			{
				fprintf(stderr, "No unique key '%s'\n", str);
				return -1;
			}
			
			found = p;
		}
	}
	
	if (!found)
	{
		fprintf(stderr, "No key matching with '%s'\n", str);
		return -1;
	}
	
	return found->val;
}

/* convert a string to upper letters */
static void strtoupper(char *str)
{
	for (; *str; str++)
		*str = toupper(*str);
}

static void usage(void)
{
	printf("alc-verb for AppleALC (based on alsa-tools hda-verb)\n");
	printf("usage: alc-verb [option] nid verb param\n");
	printf("   -d <int>  Specify device index\n");
	printf("   -l        List known verbs and parameters\n");
	printf("   -q        Only print errors when executing verbs\n");
	printf("   -L        List known verbs and parameters (one per line)\n");
}

static void list_verbs(int one_per_line)
{
	printf("known verbs:\n");
	list_keys(hda_verbs, one_per_line);
	printf("known parameters:\n");
	list_keys(hda_params, one_per_line);
	printf("known devices:\n");

	size_t nameCount = 0;
	io_string_t *names = find_services(&nameCount);
	if (names == NULL)
	{
		return;
	}

	for (size_t i = 0; i < nameCount; i++)
	{
		printf("  %zu. %s\n", i, names[i]);
	}
	free(names);
}

int main(int argc, char **argv)
{
	long nid, verb, params;
	int c;
	char **p;
	bool quiet = false;
	int dev = 0;
	
	while ((c = getopt(argc, argv, "d:qlL")) >= 0)
	{
		switch (c)
		{
			case 'd':
				dev = (unsigned)atoi(optarg);
				break;
			case 'l':
				list_verbs(0);
				return 0;
			case 'L':
				list_verbs(1);
				return 0;
			case 'q':
				quiet = true;
				break;
			default:
				usage();
				return 1;
		}
	}
	
	if (argc - optind < 3)
	{
		usage();
		return 1;
	}
	
	p = argv + optind;
	nid = strtol(*p, NULL, 0);
	if (nid < 0 || nid > 0xff) {
		fprintf(stderr, "invalid nid %s\n", *p);
		return 1;
	}
	
	p++;
	if (!isdigit(**p))
	{
		strtoupper(*p);
		verb = lookup_str(hda_verbs, *p);
		
		if (verb < 0)
			return 1;
	}
	else
	{
		verb = strtol(*p, NULL, 0);
		
		if (verb < 0 || verb > 0xfff)
		{
			fprintf(stderr, "invalid verb %s\n", *p);
			return 1;
		}
	}
	
	p++;
	if (!isdigit(**p))
	{
		strtoupper(*p);
		params = lookup_str(hda_params, *p);
		if (params < 0)
			return 1;
	}
	else
	{
		params = strtol(*p, NULL, 0);
		
		if (params < 0 || params > 0xffff)
		{
			fprintf(stderr, "invalid param %s\n", *p);
			return 1;
		}
	}

	if (!quiet)
		printf("nid = 0x%lx, verb = 0x%lx, param = 0x%lx\n", nid, verb, params);
	
	// Execute command
	uint32_t result = execute_command(dev, nid, verb, params);

	// Print result
	printf("0x%08x\n", result);

	return 0;
}

