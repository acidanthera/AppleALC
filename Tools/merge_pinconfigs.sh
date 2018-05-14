#!/bin/bash

# GenAppleALC.sh : 03/30/2016 16:51 PM
# By cecekpawon
# https://github.com/cecekpawon/AppleALC/ - Extras

if [ "$1" == "" ]; then
  cd "`dirname "$0"`"
fi

gPlistBuddyCmd="/usr/libexec/PlistBuddy -c"
AppleALC="AppleALC.kext"
ALCContents="${AppleALC}/Contents"
ALCPlist="${ALCContents}/Info.plist"
ALCPlugIns="${ALCContents}/PlugIns"
ALCPinConfigs="${ALCPlugIns}/PinConfigs.kext"
ALCPinConfigsPlist="${ALCPinConfigs}/Contents/Info.plist"
gDate=$(date +"%Y-%m-%d_%H-%M-%S")
ALCZipBkp="${AppleALC}-(${gDate}).zip"

if [[ ! -d $ALCPinConfigs || ! -f $ALCPlist || ! -f $ALCPinConfigsPlist ]]; then
 echo "Place ${0##*/} to same directory as ${AppleALC}"
 exit
fi

zip -qr $ALCZipBkp $AppleALC

$gPlistBuddyCmd "Delete ':Tmp'" $ALCPlist &>/dev/null || :
$gPlistBuddyCmd "Add ':Tmp' dict" $ALCPlist
$gPlistBuddyCmd "Merge ${ALCPinConfigsPlist} ':Tmp'" $ALCPlist
$gPlistBuddyCmd "Delete ':IOKitPersonalities:HDA Hardware Config Resource'" $ALCPlist &>/dev/null || :
$gPlistBuddyCmd "Copy ':Tmp:IOKitPersonalities:HDA Hardware Config Resource' ':IOKitPersonalities:HDA Hardware Config Resource'" $ALCPlist
$gPlistBuddyCmd "Delete ':Tmp'" $ALCPlist &>/dev/null || :
$gPlistBuddyCmd "Delete ':OSBundleLibraries:as.vit9696.PinConfigs'" $ALCPlist &>/dev/null || :

rm -rf $ALCPlugIns
