#!/bin/bash

# zlib_pack.command
# Usage: ./zlib_pack.command
#
# Created by Rodion Shingarev on 17/05/15.
#

MyPath=$(dirname "$BASH_SOURCE")
pushd "$MyPath/../" &>/dev/null

find ./Resources -name '*.xml' | xargs -P $(getconf _NPROCESSORS_ONLN) \
  -I {} sh -c 'perl Tools/zlib.pl deflate "$1" > "$1".zlib' -- {}

popd &>/dev/null
