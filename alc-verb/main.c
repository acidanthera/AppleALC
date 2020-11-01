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
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <IOKit/IOKitLib.h>

#include <CoreFoundation/CoreFoundation.h>

#include "UserKernelShared.h"
#include "hdaverb.h"

static unsigned execute_command(uint16_t nid, uint16_t verb, uint16_t param)
{
	io_connect_t dataPort;
	
	CFMutableDictionaryRef dict = IOServiceMatching(kALCUserClientProvider);
	
	io_service_t service = IOServiceGetMatchingService(kIOMasterPortDefault, dict);
	
	if (!service)
	{
		printf("Could not locate ALCUserClientProvider. Abort.\n");
		return kIOReturnError;
	}
	
	kern_return_t kr = IOServiceOpen(service, mach_task_self(), 0, &dataPort);
	
	IOObjectRelease(service);
	
	if (kr != kIOReturnSuccess)
	{
		printf("Failed to open ALCUserClientProvider service: %08x.\n", kr);
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
			fprintf(stderr, "\n");
			c = 0;
		}
		
		if (one_per_line)
			fprintf(stderr, "  %s\n", tbl->str);
		else if (!c)
			fprintf(stderr, "  %s", tbl->str);
		else
			fprintf(stderr, ", %s", tbl->str);
		
		c += 2 + len;
	}
	
	if (!one_per_line)
		fprintf(stderr, "\n");
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
	fprintf(stderr, "alc-verb for AppleALC (based on alsa-tools hda-verb)\n");
	fprintf(stderr, "usage: alc-verb [option] nid verb param\n");
	fprintf(stderr, "   -l      List known verbs and parameters\n");
	fprintf(stderr, "   -L      List known verbs and parameters (one per line)\n");
}

static void list_verbs(int one_per_line)
{
	fprintf(stderr, "known verbs:\n");
	list_keys(hda_verbs, one_per_line);
	fprintf(stderr, "known parameters:\n");
	list_keys(hda_params, one_per_line);
}

int main(int argc, char **argv)
{
	long nid, verb, params;
	int c;
	char **p;
	bool quiet = false;
	
	while ((c = getopt(argc, argv, "qlL")) >= 0)
	{
		switch (c)
		{
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
	uint32_t result = execute_command(nid, verb, params);

	// Print result
	printf("0x%08x\n", result);

	return 0;
}

