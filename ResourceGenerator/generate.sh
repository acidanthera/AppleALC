#!/bin/bash

#  generate.sh
#  AppleALC
#
#  Copyright © 2016-2017 vit9696. All rights reserved.

# Modified for ResourceGenerator

# Optimise packed layouts and platforms
. "${PROJECT_DIR}/Tools/zlib_unpack.command"
. "${PROJECT_DIR}/Tools/zlib_pack.command" -d

ret=0
"${TARGET_BUILD_DIR}/ResourceGenerator" \
	"${PROJECT_DIR}/Resources" \
	"${PROJECT_DIR}/AppleALC/kern_resources.cpp" || ret=1

if (( $ret )); then
	echo "Failed to build kern_resources.cpp"
	exit 1
fi
