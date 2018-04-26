#!/bin/bash

#  generate.sh
#  AppleALC
#
#  Copyright © 2016-2017 vit9696. All rights reserved.

# Remove the original resources
rm -f "${PROJECT_DIR}/AppleALC/kern_resources.cpp"

# Optimise packed layouts and platforms
. "${PROJECT_DIR}/Tools/zlib_pack.command"

# Reformat project plists (perl part is for Xcode styling)
find "${PROJECT_DIR}/Resources/" -name "*.plist" \
  -exec plutil -convert xml1 {} \; \
  -exec perl -0777 -pi -e "s#<data>\s+([^\s]+)\s+</data>#<data>\1</data>#g" {} \; || exit 1

ret=0
"${TARGET_BUILD_DIR}/ResourceConverter" \
	"${PROJECT_DIR}/Resources" \
	"${PROJECT_DIR}/AppleALC/kern_resources.cpp" || ret=1

if (( $ret )); then
	echo "Failed to build kern_resources.cpp"
	exit 1
fi
