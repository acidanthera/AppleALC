#!/bin/bash

# zlib_optimize.command
# Usage: ./zlib_optimize.command
#
# Created by Rodion Shingarev on 17/05/15.
#

MyPath=$(dirname "$BASH_SOURCE")
pushd "$MyPath/../" &>/dev/null

find ./Resources -name 'Platform*.xml' | while read file
do
	echo "Optimizing" $file
	#perl zlib.pl deflate "$file" > "$file.zlib"
	/usr/libexec/PlistBuddy -c "Delete :CommonPeripheralDSP" $file
	/usr/libexec/PlistBuddy -c "Add CommonPeripheralDSP array" $file
	/usr/libexec/PlistBuddy -c "Add CommonPeripheralDSP:0 dict" $file
	/usr/libexec/PlistBuddy -c "Add CommonPeripheralDSP:1 dict" $file

	/usr/libexec/PlistBuddy -c "Add CommonPeripheralDSP:0:DeviceID integer" $file
	/usr/libexec/PlistBuddy -c "Add CommonPeripheralDSP:0:DeviceType string" $file
	/usr/libexec/PlistBuddy -c "Add CommonPeripheralDSP:1:DeviceID integer" $file
	/usr/libexec/PlistBuddy -c "Add CommonPeripheralDSP:1:DeviceType string" $file

	/usr/libexec/PlistBuddy -c "Set CommonPeripheralDSP:0:DeviceType Headphone" $file
	/usr/libexec/PlistBuddy -c "Set CommonPeripheralDSP:1:DeviceType Microphone" $file
done

popd &>/dev/null
