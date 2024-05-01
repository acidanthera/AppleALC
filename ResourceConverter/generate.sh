#!/bin/bash

#  generate.sh
#  AppleALC
#
#  Copyright © 2016-2017 vit9696. All rights reserved.

# Optimise packed layouts and platforms
echo "$(date) Start packing in ${PROJECT_DIR}"
. "${PROJECT_DIR}/Tools/zlib_pack.command"
echo "$(date) Done formatting"

# Reformat project plists (perl part is for Xcode styling)
echo "$(date) Start formatting in ${PROJECT_DIR}"
find "${PROJECT_DIR}/Resources" -name "*.plist" -o -name '*.xml' | xargs -P $(getconf _NPROCESSORS_ONLN) -I {} sh -c '\
  h=$(md5 "${1}" | cut -f2 -d"=") ; \
  if [ -f "${1}.md5" ] && [ "$(cat "${1}.md5")" = "$h" ]; then \
#    echo "Skipped ${1}" ; \
    continue ; \
  else \
    plutil -convert xml1 "${1}" || exit 1 ; \
    curr=0 ; prev=1 ; \
    while [ "$curr" != "$prev" ]; do \
      prev=$(stat -f%z "${1}") ; \
      perl -0777 -pi -e "s#(?<=<data>)([^\n\s]*)[\n\s]+([^<]*)(?=</data>)#\1\2#g" "${1}" || exit 1 ; \
      curr=$(stat -f%z "${1}") ; \
    done ; \
    if [[ "${1}" != *.plist ]]; then \
      perl -0777 -pi -e "s#(<\?xml[^>]+>\n<\!DOCTYPE[^>]+>\n<plist[^>]+>\n|</plist>\n)##g" "${1}" || exit 1 ; \
    fi ; \
    h=$(md5 "${1}" | cut -f2 -d"=") ; \
    echo "$h" > "${1}.md5" || exit 1 ; \
    echo "Reformatted ${1}" ; \
  fi' -- {}
echo "$(date) Done formatting"

echo "$(date) Start building resources"
find "${PROJECT_DIR}/Resources" -name "*.md5" -exec cat "{}" + > "${PROJECT_DIR}/Resources.tmp.md5" || exit 1
h=$(md5 "${PROJECT_DIR}/Resources.tmp.md5")
if [ -f "${PROJECT_DIR}/AppleALC/kern_resources.cpp" ] && [ -f "${PROJECT_DIR}/Resources.md5" ] && [ "$h" = "$(cat ${PROJECT_DIR}/Resources.md5)" ]; then
  echo "Trusting existing kern_resources.cpp"
else
  # Remove the original resources
  rm -f "${PROJECT_DIR}/AppleALC/kern_resources.cpp"
  ret=0
  "${TARGET_BUILD_DIR}/ResourceConverter" \
    "${PROJECT_DIR}/Resources" \
    "${PROJECT_DIR}/AppleALC/kern_resources.cpp" || ret=1

  if (( $ret )); then
    echo "Failed to build kern_resources.cpp"
    exit 1
  fi
  echo "$h" > "${PROJECT_DIR}/Resources.md5"
fi

echo "$(date) End building resources"
