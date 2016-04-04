AppleALC Changelog
==================

#### v1.0.7
- Added 0x100004 revision to ALC269
- Added custom ALC269 resources for SONY VAIO VPCEB3M1R layout-id 20
- Added custom ALC282 resources for Acer Aspire E1-572G layout-id 28
- Added into ALC269 info.plist no-sleep, no-click, headphone detect patches
- Added ALC282 resources for Acer Aspire on IvyBridge by Andrey1970 layout-id 27
- Added Mirone resources to ALC892 thanks to cecekpawon
- Added ALC668 resources for DELL Precision M3800 by Syscl
- Allowed providing non-existent layouts

#### v1.0.6
- Reduced kext size by optimising capstone build options
- Added Model key with possible Laptop/Desktop/Any values to Controllers.plist (matches IODT/comptaible)
- Added VIA VT2020/2021 codec support layout-id 5, 7 Mirone resources
- Added Z97 HDEF controller patch
- Added Intel HD4000 HDMI support ig-platform-id 0x0166000A for Laptop and Desktop
- Added ALC668 layout-id 28 (for Asus N750Jk int.mic and sub working)
- Added ALC272 layout-id 3 Mirone resources
- Added ALC1150 layout-id 3 (Toleda update)
- Added missing data for ALC1150 resources
- Added ALC233 codec support layout-id 3 Mirone resources
- Added ALC235 codec support layout-id 3 Mirone resources
- Added ALC255 codec support layout-id 3 Mirone resources
- Added ALC268 codec support layout-id 3 Mirone resources
- Fixed incorrect ALC282 codec-id (should be 642)

#### v1.0.5
- Fixed AppleHDAController patching regression
- Fixed incorrect ALC269 codec-id (should be 617)
- Added embedded resource file deduplication reducing the size
- Added ALC887-VD, layout-id 11-15 codec support (by InsanelyDeepak)
- Added ALC883, layout-id 7 codec support (by Andrey1970)
- Changed resources and pinconfig for ALC888 deleted layout 1 using layout 5 for 3 ports and layout 7 for 5/6 ports 
- Added CX20590 Mirone resources layout-id 3
- Added ALC1150 Mirone resources layout-id 5 for 3 ports and layout-id 7 for 5/6 ports 
- Added ALC887-VD Mirone resources layout-id 5 for 3 ports and layout-id 7 for 5/6 ports 
- Added ALC882 codec support Mirone resources layout-id 5 for 3 ports and layout-id 7 for 5/6 ports
- Changed ALC668 resources and pinconfig Mirone resources
- Added ALC663 Mirone resources v1 - layout-id 3, v2 - layout-id 4
- Changed ALC662 resources to Mirone resources layout-id 5 for 3 ports and layout-id 7 for 5/6 ports 
- Added ALC282 Mirone resources v1 - layout-id 3, v2 - layout-id 4
- Sync PinConfig info.plist deleted unnesessary items
- Sync layout names (1,2,3 - Toleda Desktop, 1-8 - Mirone Laptop Patches, 5,7 - Mirone Desktop, 10-99 - Custom User layouts)
- Sync platforms names (platformsT - Toleda, PlatformsM - Mirone, Platforms10-99 - custom user platforms)
- Fix error in ALC282 PinConfig
- Added ALC270 Mirone resources layout-id 3, 4
- Added ALC269, ALC269-VB, ALC269-VC Mirone resources layout-id 1, 2, 3, 4, 5, 6, 7, 8
- Changed ALC269 layout-id 27 pinconfig
- Added revision 0x100001 into ALC888 info.plist and sync platforms name to common view
- Added VIA VT1802 revision 0x100000 layout-id 3 codec support Mirone resources
- Added AD1988B layout-id 5, 7 (3 ports, 5/6 Ports) Mirone resources and patch for El Capitan 8319D411->00000000
- Added AD2000B codec support layout-id 5, 7 (3 ports, 5/6 Ports) Mirone resources
- Change ALC270 layout28.xml.zlib and platforms28xml.zlib workaround for external microphone

#### v1.0.4
- Fixed a rare kernel panic due to uninitialised kern_proc
- Fixed a rare loading failure due to IOHDA value bruteforce necessity
- Added retry possibility for -alcpolicy
- Set -alcpolicy mode the default mode
- Added patch for AMD Radeon R7 370 HDMI (by John998)
- Changed platforms12.xml.zlib for ALC889 set MinimumSampleRate to 48000 (by Sergey_Galan)
- Added ALC269VC for Lenovo V850 codec support (by Ar4erR07)
- Added ALC270, layout-id 27 codec support (by m-dudarev)
- Added ALC282 DSP features and config improvements (by savvamitrofanov)
- Added ALC662, layout-id 12 codec support (by AxenovGosha)
- Added ALC663, layout-id 28 codec support (by m-dudarev)

#### v1.0.3
- Fixed AAPL,ig-platform-id naming
- Added preliminary start modes support
- Added TrustedBSD start via -alcpolicy and IOService::start via -alciokit arguments
- Allowed kext patches with 0 count replacing all entries (use this with a reason)
- Fixed ALC270 regressions
- Added ALC889, layout-id 12 codec support (by Sergey_Galan)
- Added ALC887-VD, layout 99 codec support (by Constanta)
- Removed unnecessary NVIDIA GT240 HDMI patches

#### v1.0.2
- Added Controller.plist with controller-specific patches
- Added Intel X99 and Intel HD4600 controller patches
- Reverted device-id comparison code, it was redundant
- Added prelinkedkernel reading (used exclusively in OS X Installer/Recovery HD)
- Added AppleIntelFramebufferAzul patches to fix HD4600 audio
- Added ALC269 10.9.x/10.10.x patches (by Andrey1970)
- Added ALC269 for Asus K53SJ (by alex1960)
- Added ALC270 codec support (by m-dudarev)
- Added ALC270 for Asus K56CB (by m-dudarev)
- Added ALC668 for Asus N750Jk (by alex1960)
- Added ALC885 (by alex1960)
- Added NVIDIA GT240 HDMI patches (by Vandroiy)
- Added Intel HD4000 patches (by Andrey1970)

#### v1.0.1
- boot-args presence is now optional
- Platforms must conform to layout-id from now on
- Workaround a race-condition kernel panic when accessing current_thread credential
- Fixed a kernel panic when kaslr is disabled
- Renamed control boot arguments to -alcdbg and -alcoff
- Added optional device-id comparison to patch and file matching
- Added PinConfs plugin injecting pin configurations
- Added AD1988B codec support (by Rodion)
- Added ALC269VC (Lenovo Z580) codec support (by John)
- Added ALC269 (Acer Aspire) codec support (by Andrey1970)
- Added ALC282 codec support (by S.Mitrofanov)
- Added ALC887 codec support (by x-tropic)
- Added ALC888 codec support (by Vandroiy)
- Added ALC889 codec support (by Ukr55)
- Added ALC892 codec support (by FredWst, m-dudarev)
- Added ALC898 codec support (by Andrey1970)
- Added CX20590 codec support (by usr-sse2)

#### v1.0.0
- Initial release
