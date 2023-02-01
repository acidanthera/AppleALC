#!/bin/sh
#
#  wiki_table.command
#
#  Created by Rodion Shingarev on 25.03.21.
#  

outfile=~/Desktop/SupportedСodecs.md
printf '*Thеse tables are generated using [wiki_table.command](https://github.com/acidanthera/AppleALC/blob/master/Tools/wiki_table.command)* \n' > $outfile
printf '#### Currently supported codecs '$(date '+%Y-%m-%d')'\n' >> $outfile
printf '| Vendor | Codec | Revisions and layouts | MinKernel | MaxKernel |\n' >> $outfile
printf '|--------|-------|-----------------------|-----------|-----------|\n' >> $outfile

cd "$(dirname "$0")/../Resources/"
for d in */ ; do
if [ -f "$d"/Info.plist ]; then
vendor=$(/usr/libexec/PlistBuddy -c "Print Vendor"  "$d"/Info.plist)

i=0
version1=20
version=20
until [ -z $version ]
do
version=$(/usr/libexec/PlistBuddy -c "Print Patches:$i:MinKernel"  "$d"/Info.plist)
if [[ $version -lt $version1 ]] && [ ! -z $version ]
then
version1=$version
fi
((i=i+1))
done

let j=0
rev=""

while [ $j -ge 0 ]
do
revision=$(/usr/libexec/PlistBuddy -c "Print Revisions:$j"  "$d"/Info.plist)
if [ ! -z $revision ]
then
rev=$rev" "$(printf %#0x, $revision)
((j=j+1))
else
let j=-1
fi
done


if [[ $i -eq 1 ]]
then
ver=" — "
else
let version2=$version1-4
ver=$version1' (10.'$version2')'
fi

case ${d%/} in
CX8070)
d1='CX8070/CX11880/'
;;
CX20751_2)
d1='CX20751/CX20752/'
;;
CX20753_4)
d1='CX20753/CX20754/'
;;
ALC225)
d1='ALC225/ALC3253/'
;;
ALC233)
d1='ALC233/ALC3236/'
;;
ALC255)
d1='ALC255/ALC3234/'
;;
ALC256)
d1='ALC256/ALC3246/'
;;
ALC269)
d1='ALC269/ALC271X/'
;;
ALC290)
d1='ALC290/ALC3241/'
;;
ALC888)
d1='ALC888/ALC1200/'
;;
ALC891)
d1='ALC891/ALC867/'
;;
ALC898)
d1='ALC898/ALC899/'
;;
VT2020_2021)
d1='VT2020/VT2021/'
;;
IDT92HD66C3_65)
d1='IDT92HD66C3/65/'
;;
IDT92HD87B1_3)
d1='IDT92HD87B1/3/'
;;
IDT92HD87B2_4)
d1='IDT92HD87B2/4/'
;;
*)
d1=$d
;;
esac


printf '| '$vendor' ' >> $outfile
printf '| ['${d1%/}'](https://github.com/acidanthera/AppleALC/tree/master/Resources/'${d%/}')' >> $outfile
printf '| '"${rev}"' layout ' >> $outfile
cd $d
u=""
for l in $(ls layout*.xml | sed 's/[^0-9]//g' |  sort -n) ; do u="${u:+$u, }$l"; done
printf "${u}" >> $outfile
printf '| '"${ver}"' | — | \n' >> $outfile
cd ..
fi
done

printf '\n\n' >> $outfile
printf '#### Controllers patches\n' >> $outfile
printf '| Vendor | Patch for not native | Device | Model | MinKernel | MaxKernel |\n' >> $outfile
printf '|--------|----------------------|--------|-------|-----------|-----------|\n' >> $outfile
cd "$(dirname "$0")/Resources/"
f='Controllers.plist'
let k=0
while [ $k -ge 0 ]
do
vendor=$(/usr/libexec/PlistBuddy -c "Print $k:Vendor"  "${f}")
name=$(/usr/libexec/PlistBuddy -c "Print $k:Name"  "${f}")
device=$(/usr/libexec/PlistBuddy -c "Print $k:Device"  "${f}")
model=$(/usr/libexec/PlistBuddy -c "Print $k:Model"  "${f}")
if [ ! -z $device ]
then
printf '| '"${vendor}"' ' >> $outfile
printf '| [%s](https://github.com/acidanthera/AppleALC/blob/master/Resources/Controllers.plist) | 0x%04X' "${name}" "${device}" >> $outfile

if [ -z $model ]
then model=" — "
fi
printf '| '"${model}"' ' >> $outfile


i=0
version1=20
version=20
until [ -z $version ]
do
version=$(/usr/libexec/PlistBuddy -c "Print $k:Patches:$i:MinKernel"  "${f}")
if [[ $version -lt $version1 ]] && [ ! -z $version ]
then
version1=$version
fi
((i=i+1))
done

if [[ $i -eq 1 ]]
then
ver=" — "
else
let version2=$version1-4
ver=$version1' (10.'$version2')'
fi

i=0
max1=12
max=12
until [ -z $max ]
do
max=$(/usr/libexec/PlistBuddy -c "Print $k:Patches:$i:MaxKernel"  "${f}")
if [[ $max -gt $max1 ]] && [ ! -z $max ]
then
max1=$max
fi
((i=i+1))
done

if [[ $i -eq 1 ]]
then
maxver=" — "
else
let max2=$max1-4
maxver=$max1' (10.'$max2')'
fi

if [ $device -eq 3084 ]
then
maxver=" — "
fi


printf '| '"${ver}"' | '"${maxver}"' | ' >> $outfile
printf '\n' >> $outfile
((k=k+1))
else
let k=-1
fi
done
