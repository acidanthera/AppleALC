#!/bin/bash

# dump_coeff.sh - Dump processing caps for node in macOS using alc-verb.
#
# Copyright (c) 2022, Michael Beaton. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-3-Clause
#

if [ -z "$1" ] || [ -z "$2" ] || [ -z "$3" ] || [ ! -z "$4" ]
then
    echo "Usage: dump_coeff {device-index} {node-id} {count}"
    exit 0
fi

if ! command -v alc-verb &> /dev/null
then
    echo "alc-verb is not installed in path"
    exit -1
fi

echo "Node $2"
echo "  Processing caps: ncoeff=$(($3))"

for (( i = 0; i < $(($3)); i++ ))
do
    alc-verb -d $1 $2 SET_COEF_INDEX $i 2>/dev/null >/dev/null
    coeff=$(alc-verb -d $1 $2 GET_COEF_INDEX 0 2>/dev/null | tail -n 1)
    value=$(alc-verb -d $1 $2 GET_PROC_COEF 0 2>/dev/null | tail -n 1)
    printf "    Coeff 0x%02x: 0x%04x\n" $coeff $value
done
