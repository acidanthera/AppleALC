#!/bin/bash

# zlib_pack.command
# Usage: ./zlib_pack.command
#
# Created by Rodion Shingarev on 17/05/15.
#

MyPath=$(dirname "$BASH_SOURCE")
pushd "$MyPath/../" &>/dev/null

find ./Resources -name "*.xml" | xargs -P $(getconf _NPROCESSORS_ONLN) -I {} sh -c '\
  h=$(md5 "${1}" | cut -f2 -d"=") ; \
  if [ -f "${1}.zlib" ] && [ -f "${1}.zlib.md5" ] && [ "$(cat "${1}.zlib.md5")" = "$h" ]; then \
#    echo "Skipped ${1}" ; \
    continue ; \
  else \
    perl Tools/zlib.pl deflate "$1" > "${1}.zlib" || exit 1 ; \
    echo "$h" > "${1}.zlib.md5" || exit 1 ; \
    echo "Packed ${1}" ; \
  fi' -- {}

popd &>/dev/null
