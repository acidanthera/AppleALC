#!/bin/bash

# zlib_unpack.command
# Usage: ./zlib_unpack.command [-d]
#
# Created by Rodion Shingarev on 17/05/15.
#

MyPath=$(dirname "$BASH_SOURCE")
pushd "$MyPath/../" &>/dev/null

find ./Resources -name '*.xml.zlib' | while read file
do
	echo "Decompressing" $file
	perl Tools/zlib.pl inflate "$file" > "${file%.*}" || exit 1

	# Enforce human readable form
	out=$(plutil -convert xml1 "${file%.*}" 2>&1)
	if [ "$out" != "" ]; then
		>&2 echo "Warning: $out"
		exit 1
	fi
done

popd &>/dev/null
