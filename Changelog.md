AppleALC Changelog
==================
#### v1.3.4
- Disabled AppleGFXHDA matching onto HDEF to force AppleHDA usage

#### v1.3.3
- Added ability to disable controller patching by injecting property 'no-controller-patch' (for use of FakePCIID_Intel_HDMI_Audio)
- Fixed volume control for CX8200 with Wern Apfel patch from CX20724
- Fixed patches for IDT92HD872_4 (IDT 76d9)
- Added ALC235 layout-id 14 for Lenovo Y520/Y720 series by the-braveknight
- Added ALC230 layout-id 20 for Lenovo Ideapad 320 by maiconjs
- Added ALC887-VD AD0 layout-id 20 for Asus Z97M-PLUS/BR by maiconjs
- Added ALC887-VD layout-id 40 for Asus B85-ME by maiconjs
- Added ALC236 layout-id 16 for Lenovo Ideapad 320s 14ikb by RodionS
- Added ALC885 layout-id 15 for GA-G33M-DS2R by MacPeet
- Added ALC662 layout-id 15 for Acer Aspire A7600U All in One by MacPeet
- Added ALC255 layout-id 30 for XiaoMi Notebook Air by HongyuS
- Added CX20590 layout-id 13 and 14 for Lenovo T420 by tluck
- Added ALC283 layout-id 44 for ThinkCentre M93z 10AF ALC283 by giesteira
- Added ALC662v3 layout-id 16 for Lenovo ThinkCentre M92P SFF by phucnguyen.2411
- Added ALC282 layout-id 22 for Lenovo Y430P  by lovert
- Fixed CX20756 layout-id 13
- Added ALC892 layout-id 15 for MSi Z97S SLI Krait Edition by MacPeet
- Fix volume control for CX20756 layout-id 13 and external mic minor changes/fixes

#### v1.3.2
- Added ALC269 layout-id 45 for Asus A45A and K45 by maiconjs (Wolfield)
- Added WakeConfigData for ALC892 layout-id 1 by FredWst
- Added ALC295 layout-id 14 for Asus UX430UA by InsanelyDeepak
- Added ALC295 layout-id 15 for ALC295/ALC3254 by InsanelyDeepak
- Remove duplicate resources for ALC295 layout-id 12 on Asus UX430
- Added ALC230 layout-id 13 for Lenovo 310-14ISK by Andres

#### v1.3.1
- Fixed incorrect unused HDAU termination
- Fixed pinconfig refresh causing random init failures
- Added ALC225/ALC3253 layout-id 30 for Dell Inspiron 17-7779 by Constanta
- Added ALC295 layout-id 12 for Asus UX430 by hieplpvip
- Added Realtek ALC256 (3246) layout-id 11 for dell 7559 by Rockjesus
- Added ALC887 layout-id 50 for Asus PRIME B250-PLUS by 0th3r
- Added ALC295 layout-id 1 for HP Envy x360 15-bp107tx by Damon

#### v1.3.0
- Requires Lilu 1.2.5 and higher and enabled on 10.14 by default
- Added digital audio property injection for AMD, Intel, NVIDIA
- Added NVIDIA multi-gpu NVIDIA HDAU patch support
- Added ALC269VC layout-id 30 for Hasee Z6SL7R3 by HF

#### v1.2.8
- Added basic 10.14 compatibility via masking real `layout-id`
- AppleALC selected `layout-id` is now shown as `alc-layout-id` in ioreg
- Overriden `layout-id` can be passed as`apple-layout-id` (`alcaaplid=X`)
- Added ALC298 layout-id 66 for MECHREVO S1 by lgs3137
- Added ALC298 revision-id 0x100101 support
- Added ALC892 layout-id 12 for MSI GP70/CR70 by Slava77
- Added ALC257 layout-id 11 for Lenovo T480 by MacPeet
- Added ALC294 layout-id 12 for Lenovo M710Q by MacPeet
- Added ALC298 layout-id 30 for Xiaomi Mi Notebook Air 13.3 by Constanta

#### v1.2.7
- Added NVIDIA HDMI patch for GP107 and GP108 for 10.13.4+
- Added NVIDIA HDMI patch for GP104 for 10.13.4+
- Added ALC898 layout-id 98 for MSI GE62 7RE Apache Pro by spectra
- Added ALC1220 layout-id 13 for Clevo P950HR by MacPeet
- Added ALC888 layout-id 27 for Acer Aspire 7738G by MacPeet
- Added 300 Series PCH HD Audio Controller patch by chrome
- Added ALC269 layout-id 66 for Clevo N155RD by DalianSky
- Added ALC285 layout-id 11 for Lenovo ThinkPad X1C6th by Rover
- Added ALC294 layout-id 11 for Asus FL8000U by Rover
- Added ALC269 layout-id 58 for HASEE Z6-i78154S2 by lianny
- Added ALC233 layout-id 32 for ASUS VIVOBOOK S301LA by MacPeet

#### v1.2.6
- Fixed a rare kernel panic due to use-after-free in verb code at waking from sleep

#### v1.2.5
- Implemented verb execution after wake (via `WakeVerbReinit` in PinConfigs), obsoletes CodecCommander in fixed resources
- Implemented custom verbs for wake  (via `WakeConfigData` in PinConfigs), `ConfigData` is used by default
- Merged PinConfigs into AppleALC main binary by default for compatibility reasons
- Hardened solved symbol verification to avoid panics with broken kext cache
- Added PinConfigurations injection, since they provide useful debugging info
- Added NVIDIA HDMI patch for GM204 for 10.13.4+
- Added ALC233 layout-id 33 for ASUS VIVOBOOK S451LA by MacPeet
- Added HD3000 desktop and laptop HDMI patches
- Added wake verbs for ALCS1220A layout-id 1, 2, 5, 7 to avoid the need of CodecCommander
- Added EAPD wake verbs to all ALC 221, 225, 233, 235, 236, 255, 256, 270, 271, 272, 284, 286, 288, 290, 293, 295 by Vandroiy
- Added EAPD wake verbs to some ALC 269, 275, 280, 282, 283, 292, 294, 298, 662, 663, 668, 887, 888, 889, 892, 898, 1150 by Vandroiy
- Fixed broken AFGLowPowerState, LayouID and other fields in several PinConfigs info.plist
- Fixed rare kernel panics due to short incompatible HDEF controller patches affecting other code

#### v1.2.4
- Added ALC269 layout-id 10 (toleda) for Brix by ttimasdf
- Added ALC663 layout-id 99 for ASUS N71J by alex1960
- Added CX20641 layout-id 11 and layout-id 13 for Dell OptiPlex 3010 or compatible by MacPeet
- Added CX20642 layout-id 11 and layout-id 13 for Fujitsu ESPRIMO E910 E90+ or compatible by MacPeet
- Added NVIDIA HDMI patches for GK208, GM206, GP102, GP106 for 10.13.4+ by FredWst
- Added Intel HDMI patches for Haswell 0x0A260005  by Andrey1970
- Added NVIDIA HDMI patch for GM200 by al3xtjames

#### v1.2.3
- Added `alcid=layout` boot argument for testing purposes
- Added automatic rename for HDEF device
- Added ALC255 layout-id 18 for Aorus X5V7 by DuNe
- Change Platforms18.xml.zlib for ALC255 by DuNe
- Added ALC898 layout-id 101 by Andrey1970 (4 Line Out)
- Added ALC262 layout-id 28 for HP Z800-Z600 series by MacPeet
- Added ALC225 layout-id 33 by ChalesYu
- Added ALC282 layout-id 127 No input boost Acer Aspire on IvyBridge by Andrey1970
- Added ALC294 layout-id 13 by InsanelyDeepak
- Added IDT92HD91BXX layout-id 13 for HP Envy 6 1171-SG by MacPeet

#### v1.2.2
- Added ALC256 layout-id 56 for Dell 7000 by DalianSky
- Added ALC298 layout-id 47 for ThinkPad T470P by DalianSky
- Update ALC269 layout27.xml.zlib by Andrey1970
- Added ALC269 layout-id 93, 99 and 127 by Andrey1970 (No input boost - no noise in Siri)
- Added ALC671 layout-id 12 for Fujitsu-Siemens D3433-S (Q170 chip) by MacPeet
- Added ALC280 layout-id 13 and 15 for DELL T20 by MacPeet
- Added ALC262 layout-id 13 for Fujitsu Celsius H270 by MacPeet
- Added ALC891 layout-id 11 for HP Pavilion Power 580-030ng by MacPeet
- Added ALC282 layout-id 76 for Hasee K580C by YM2008
- Fix desktop HD4000 id by Andrey1970
- Added IDT92HD91BXX layout-id 84 for HP Elitebook 840 G1 by macish
- Added ALC295 layout-id 13 for Dell7570 by DalianSky
- Added ALC298 layout-id 99 for XiaoMIPro by DalianSky
- Added ALC283 layout-id 1 for NUC/BRIX by Toleda
- Added ALC255 (3246) layout-id 99 for XiaoMi Air by DalianSky
- Added ALC236 layout-id 15 for Lenovo Ideapad 500-15ISK by MacPeet
- Added ALC892 layout-id 98 with working SPDIF

#### v1.2.1
- Added ALC298 layout-id 11 for Alienware 17 (ALC3266) by Rockjesus
- Added VIA VT2020/2021 layout-id 9 by SonicBSV
- Added ALC269 layout-id 18 for Thinkpad X230 by Hypereitan
- Added ALC235 layout-id 28 for Lenovo Legion Y520 by vusun123
- Added ALC898 layout-id 65 for CLEVO P65xRS(-G) by datasone
- Added ALC298 layout-id 29 for Lenovo X270 by vusun123
- Added ALC292 layout-id 18 for Dell Precision M4800/M6800 by vanquybn
- Added IDT92HD87B1/3 layout-id 13 by InsanelyDeepak

#### v1.2.0
- Requires Lilu 1.2.0 and higher
- Added Lilu 1.2.0 safe mode loading compatibility
- Added Intel Broadwell HD6000 HDMI audio support (ig-platform-id 0x16260004)
- Added ALC262 layout-id 12 for HP Compaq dc7700 SFF by Goldfish64
- Added Intel 200 Series Mobile HDA controller 0xA171 support
- Added IDT92HD81B1C5 layout-id 11 for Dell Latitude E6410 by Goldfish64
- Added ALC898 layout-id 99 for MSI GP62 Leopard Pro by edward-p
- Correct issues with layout 11 for IDT92HD81B1C5 for Dell Latitude E6410
- Added ALC221 layout-id 11 for HP Compaq Pro 4300/Pro 6300/Elite 8300 by Goldfish64
- Added ALC892 layout-id 31 for Clevo P65xSE/SA by Derek Zhu
- Added Conexant CX20585 layout-id 13 for Toshiba L755-16R by Constanta
- Added ALC889 layout-id 11 for Medion P4020 D by MacPeet
- Update ConfigData for ALC898 layout-id 99
- Update ALC269 layout-id 16 by MacPeet

#### v1.1.4
- Added High Sierra to the list of compatible OS
- Added ALC295/ALC3254 layout-id 3 by Mirone
- Added ALC298 layout-id 72 for Dell XPS 9560 by KNNSpeed
- Added ALC236 layout-id 3 by Mirone
- Added ALC236 layout-id 13 for Lenovo Air 13 Pro by rexx0520
- Added a workaround for xnu printf limitations
- Added ALC235 layout-id 11 for Ienovo by soto2080
- Disabled redundant warnings for non-debug mode by PMheart
- Added AMD R9 290X HDMI patches
- Fix ALC236 layout 13's Headphone amplification
- Added ALC260 layout-id 11 for Fujitsu Celsius M 450 by MacPeet
- Added AD1984A layout-id 13 for Dell Optiplex760/780 by MacPeet
- Added ALC269VB layout-id 16 for Dell Optiplex 790 by MacPeet

#### v1.1.3
- Fixed ALC889 info.plist min/max kernel
- Added Desktop Intel Kaby Lake HD630 HDMI audio support (ig-platform-id 0x59120000)
- Added Conexant CX20756 layout-id 13 by InsanelyDeepak
- Added ALC282 layout-id 13 by InsanelyDeepak
- Fixed separate layout/platform injection
- Added AppleHDA8086_9D70Controller for 10.13
- Changed ALC255 layout-id 13 & 17 by InsanelyDeepak
- Fixed typo in ALC255 CodecID
- Fixed audio reenable after kextunload/kextload AppleHDA
- Added `-alcdhost` argument to enable custom audio drivers in 10.12+ (e.g. Audirvana)
- Added ALC255 layout-id 27 for Asus X556UA m-dudarev

#### v1.1.2
- Added ALC225 layout-id 28 for Dell Inspirion 7579 by ChalesYu
- Added ALC298 layout-id 28 for Dell XPS 9x50 by vusun123
- Changed ALC290 layout-id 28 resources Mic & Line In Fix by vusun123
- Added VIA VT1802 layout-id 33 for asus S400CA by ChalesYu
- Added IDT92HD91BXX layout-id 33 for HP Envy by jl4c 
- Added ALCS1220A layout-id 1, 2 for ASUS 200 series by Toleda
- Added ALC1220 layout-id 1, 2 for 200 series by Toleda
- Added ALC662 layout-id 12 for Lenovo ThinkCentre M800 by stich86 
- Added ALC892 layout-id 28 for Clevo P751DMG by crysehillmes
- Added ALC269VC layout-id 40 for Lenovo W530 by vusun123
- Added ALC280 (ALC3220) layout-id 11 for Alienware alpha (ONLY SPDIF) by ganxiao
- Added ALC662 layout-id 13 by Vandroiy for Asus X66Ic, Asus K61Ic, Asus K70Ic
- Added SPDIF support to ALC887 layout-id 99
- Added Conexant CX20590 revision-id 0x100000
- Added revision-id 0x100002 in ALC670 info.plist
- Added ALC288 layout-id 13 for Dell XPS 9343 by InsanelyDeepak
- Added ALC286 revision-id 0x100003
- Added OSBundleCompatibleVersion

#### v1.1.1
- Requires Lilu 1.1.0 or newer
- Fixed IDT92HD71B7X macOS 10.12 compatibility issue
- Added IDT92HD91BXX revision-id 0x100303
- Added CX20722 menubar patch for 10.10 and above by Wern Apfel
- Added CX20724 menubar patch for 10.10 and above by Wern Apfel
- Added CX20751/2 menubar patch for 10.10 and above by Wern Apfel
- Updated ALC269VB layout-id 11 for Dell Precision Workstation T1600 by Mosser
- Added ALC1220 layout-id 5 and 7 by Mirone
- Added ALCS1220A layout-id 5 and 7 by Mirone
- Increased replacement count for several SKL hdmi patches to workaround matching sequence issues by Andrey1970
- Update Intel HD4000 HDMI patches for 0x01660009 and 0x0166000A by Andrey1970
- Implemented simple byte sequence deduplication to reduce kext size
- Update pinconfig for ALC1220 layout-id 11 by truesoldier
- Added IDT92HD71B7 menubar patch for 10.10 and above by Wern Apfel
- Added IDT92HD91BXX ambient noise reduction to layout-id 12
- Added ALC887 revision-id 0x100202
- Changed ALC255 layout-id 3 resources by Mirone. It should fix kernel task loading issue
- Added CA0132 layout-ids 0-6 by Wern Apfel 
- Fix CX20756 typo in layout3.xml.zlib

#### v1.1.0
- AppleALC now has its kernel patcher moved to [Lilu.kext](https://github.com/vit9696/Lilu)
- Added Intel 200 Series Audio controller support by chrome
- Fixed typo in PinConfigs.kext for ALC233 layout-id 29
- Added 10.8 support to ALC282
- Added ALC286 layout-id 3 initial support by Mirone
- Added ALC888S-VD layout-id 29 for MedionP8610 by MacPeet
- Added IDT 92HD73C1X5 layout-id 19 for Dell Studio 1535 by chunnann
- Added ALC269 layout-id 19 for Asus Vivobook S300CA by chunnann
- Added ALC283 layout-id 66 for ASRock DeskMini 110(H110M-STX) by licheedev
- Optimize and compress ALC283 resources from previous commit
- Added ALC256 layout-id 28 for Asus X555UJ by vusun123
- Added Realtek ALC1220 initial support layout-id 11 by truesoldier
- Update PinConfig for ALC1220 by truesoldier

#### v1.0.19
- Added ALC269 layout-id 35 for Samsung NP350V5C-S0URU by Mirone
- Added Conexant CX20755 layout-id 3 by Mirone
- Added ALC269VB layout-id 76 for ENZ C16B by jimmy19990
- Added UserPatches.plist for internal testing
- Added ALC269VB layout-id 11 for Dell Precision Workstation T1600 by Mosser
- Recovered Creative CA0132 macOS 10.12 patch due to caused issues
- Maximised zlib resource compression level
- Added stripping of tabs and new lines in resources
- Added ALC236 layout-id 11 Initial support by Jake Lo (forum.osxlatitude)
- Added ALC269VC layout-id 14 for Samsung NT550P7C-S65 with subwoofer 2.1ch by Rockjesus
- Added Laptop Intel Skylake HD530 HDMI audio support (ig-platform-id 0x191B0000)
- Added revision-id 0x100001 into ALC236 info.plist 
- Added ALC293 codec support layout-id 28, 29 for Lenovo T460/T560 by tluck
- Added Conexant CX20751_2 HDMI/DisplayPort Output fix by syscl
- Added ALC282 layout-id 29 for Dell Inspirion 3521 by Generation88
- Added ALC293 layout-id 11 for Dell E7450 by Andres ZeroCross 
- Update PinConfig data for ALC269 layout-id 27 by Andrey1970
- Added ALC3236 (ALC233) layout-id 29 for Asus X550LDV by Mirone
- Added Conexant CX20722 codec support layout-id 3 by Mirone
- Added Conexant CX20753/4 codec support layout-id 3 by Mirone
- Added Conexant CX8050 codec support layout-id 3 by Mirone
- Added Conexant CX8200 codec support layout-id 3 by Mirone
- Added revision-id 0x100103 for ALC885
- Added ALC269VB layout-id 15 for Dell Optiplex 790 by MacPeet
- Allowed building for 10.8 without using 10.8 SDK

#### v1.0.18
- Improved operating system detection
- Decent workaround for 10.12 issues and panics
- Added ALC888S-VD layout-it 11 and 28 for Medion P9614 and E7216 by MacPeet
- Added initial support AD1984 layout-id 11 for IBM_Lenovo_ThinkPad_T61 by MacPeet
- Added ALC272 layout-id 11 for Lenovo B470 by Sam Chen
- Added Laptop Intel Broadwell HD5500/5600 HDMI audio support (ig-platform-id 0x16260006)
- Added Laptop Intel Skylake HD515/520/530 HDMI audio support (ig-platform-id 0x191E0000, 0x19160000)
- Added Desktop Intel Skylake HD530 HDMI audio support (ig-platform-id 0x19120000)
- Fixed Creative CA0132 macOS 10.12 compatibility issue
- Added ALC892 layout-id 92 for GA-Z87-HD3 by BIM167
- Added ALC295 codec support layout-id 28 for Skylake HP Pavilion by vusun123
- Added IDT92HD81B1X5 layout-id 20 and 21 by Sergey_Galan for HP ProBook 4520s and HP DV6-6169er
- Added IDT92HD81B1X5 layout-id 28 by Gujiangjiang for HP Pavilion g4 1000 series

#### v1.0.17
- Added C610/X99 Audio Controller r1 support by masking to 8ca0
- Added C610/X99 Audio Controller r2 support by masking to 8ca0
- Disabled Z97 controller patching on 10.10 and newer because it is supported by default
- Fixed ALC272 macOS 10.12 compatibility issue
- Fixed ALC885 and ALC888 macOS 10.12 compatibility issue
- Added ALC888 revision-id 100302
- Fixed CX20585 macOS 10.12 compatibility issue
- Fixed CX20588, CX20590 and CX20724 macOS 10.12 compatibility issue
- Fixed CX20757 macOS 10.12 compatibility issue
- Fixed rest of Conexant codecs for macOS 10.12 compatibility
- Added missing zero patch in IDT 92HD81B1X5 info.plist
- Added ALC892 layout-id 99 for DNS P150EM by Constanta
- Added ALC898 layout-id 5 and 7 by Mirone
- Changed logging to be done by IOLog since printf is broken on 10.12
- Added ALC3236 layout-id 28 for Asus TP500LN by Mohamed Khairy
- Fixed ALC235, ALC260, ALC280, ALC284, ALC288, ALC290, ALC292 macOS 10.12 compatibility issue
- Added ALC233 layout-id 27 for SONY VAIO Fit 14E(SVF14316SCW) by SquallATF
- Fixed all Conexant codecs macOS 10.12 compatibility issue
- Added IDT 92HD81B1X5 layout-id 11 by Andres ZeroCross
- Added ALC887 layout-id 33 for GA-Q87TN by klblk
- Added ALC256 (3246) codec support layout-id 13 by InsanelyDeepak
- Added ALC255 layout-id 13 and 17 by InsanelyDeepak
- Fixed an extremely rare crash on 10.12
 
#### v1.0.16
- Fixed a rare lock acquisition issue on 10.12
- Fixed a rare kernel panic on initialisation failure
- Improved ALC887 layout-id 11, 13 by InsanelyDeepak
- Added ALC269VC layout-id 33 for NP530U3C-A0F by BblDE3HAP
- Added ALC898 layout-id 12 with autodetect disabled by D-an-W
- Deleted not working ALC233 layout-id 12
- Added ALC233 layout-id 13 for Asus X550LC by InsanelyDeepak
- Added ALC662 layout-id 11 for Lenovo ThinkCentre M8400t-N000 by Irving23
- Fixed ALC255 macOS 10.12 compatibility issue
- Changed ALC898 layout-id 12 to 28 due to compatibility issue
- Fixed ALC275 macOS 10.12 compatibility issue
- Added ALC255 layout-id 28 for Lenovo B470 by vusun123
- Added Atom Z36xxx/Z37xxx Audio Controller support by masking to 0c0c

#### v1.0.15
- Added macOS 10.12 to the list of supported systems
- Added ALC262 codec support layout-id 11 by MacPeet
- Added ALC898 layout-id 13 for MSI GS40 by InsanelyDeepak
- Added ALC887 layout-id 18 by InsanelyDeepak
- Several optimisations to ALC887 resources by InsanelyDeepak
- Removed not working ALC887 layout-id 12 by InsanelyDeepak
- Added revision-id in IDT 92HD91BXX info.plist
- Fixed an undefined behaviour when failing to perform an i/o operation
- Guaranteed null termination for readFileToBuffer buffers

#### v1.0.14
- Added ALC290 layout-id 28 for Dell Vostro 5480 by vusun123
- Added ALC292 layout-id 28 for Lenovo T440 vusun123
- Added a few more fixes to lock acquisition panic on 10.12 Beta
- Fixed VT2020/2021 patches for 10.12 Beta by melxman
- Fixed Creative CA0132 patches for 10.12 by nmano
- Added revision-id 0x100002 for Conexant CX20590 
- Added ALC233 layout-id 12 for Asus X550LC
- Added revision-id into ALC233 info.plist
- Added CX20590 layout-id 28 for Dell Vostro 3x60 by vusun123
- Added IDT 92HD81B1X5 layout-id 12 for HP Elitebook 8460P by RehabMan
- Added IDT 92HD81B1C5 codec support layout-id 3 by Mirone
- Added ALC298 layout-id 3 with ComboJack by Mirone
- Added ALC233/3236 layout-id 5 by Mirone
- Added ALC1150 layout-id's 1, 2, 3 by Toleda

#### v1.0.13
- Improved workaround for 10.12 Beta issues
- Fixed PAGE FAULT panic on 10.12 Beta
- Added version information on boot
- Added ALC898 layout-id 11 for MSI GT72S 6QF-065CN by Irving23
- Added Conexant CX20724 layout-id 13 by InsanelyDeepak
- Added Cirrus Logic CS4210 codec support layout-id 13 by InsanelyDeepak
- Added Realtek ALC275 layout-id 13 by InsanelyDeepak
- Added Realtek ALC891 codec support layout-id 13 by InsanelyDeepak
- Added IDT 92HD90BXX layout-id 12 for Dell Latitude E6x20/Dell Precision Mx600 by vusun123
- Added ALC275 layout-id 28 for Sony Vaio by vusun123
- Added ALC298 codec support layout-id 13 by InsanelyDeepak
- Added AD1984A codec support layout-id 11 by MacPeet
- Added revision-id into AD1984A info.plist
- Fixed lock acquisition panic in iokit routins on 10.12 Beta
- Removed ALC1150 layouts 1-3 to reduce kext size
- Added ALC1150 layout 11 with boosted microphone (built on 7)

#### v1.0.12
- Lowered minimal OS requirement to 10.8 for ALC269
- Added ALC269 revision-id 0x100203 support
- Workaround for booting in 10.12 Beta
- Added a new version of null (8319D411) patch (now 8A19D411) for 10.12
- ALC269 deleted unnecessary macOS Sierra patch
- ALC887 added macOS Sierra patch
- Added revision-id 0x100005 to ALC275
- Added ALC271x layout-id 31 for Acer Aspire s3-951
- Added IDT 92HD90BXX layout-id 3 by Mirone
- Changed ALC233 layout-id 3 resources
- ALC662 deleted unnecessary macOS Sierra patch
- Added layout-id 9, 10 to CA0132 by Andres ZeroCross
- Fixed MuteGPIO in ALC3236(ALC233) by nguyenlc1993
- Disabled by default on unsupported operating systems (e.g. 10.12), use -alcbeta boot arg to override this
- Added ALC269 layout-id 32 for Samsung np880z5e-x01ru by Constanta
- Added CX20561 layout-id 3 by depingzheng
- Changed CX20561 layout 3 -> layout 11
- Optimized CX20561 platforms11.xml.zlib

#### v1.0.11
- Added ALC668 layout-id 29 for Asus N750JV by DroID969
- ALC887VD Optimization Layout 11 officially will Support AC97 Front Panel like Windows Driver
- ALC887VD Optimization Added Layout 11, 12, 13, 17
- ALC887VD Optimization Layout 11 Front Mic and Back Mic Fix
- ALC887VD Optimization Layout 12 LinIn Fix
- ALC887VD Optimization Added AMD Support in all Layouts
- ALC887VD Optimization replace several custom platforms.xml.zlib with common platformsID.xml.zlib
- Added ALC233 layout-id 4 for ALC3236 codec 
- Added ALC290 layout-id 4 for ALC3241 codec
- Added IDT92HD91BXX layout-id 12 by RehabMan for HP Envy laptops
- Changed ALC665 resources by InsanelyDeepak 
- Added ALC269VC layout-id 13 for Samsung NP350V5C-S08IT by InsanelyDeepak
- Changed ALC233 resources layout-id 4 for ALC3236 codec
- Added ALC888 layout-id 4 for Laptop by Mirone
- Added ALC892 layout-id 4 for Laptop by Mirone
- Added ALC269VB layout-id 9 for Toshiba Satellite C660-1DR

#### v1.0.10
- Added ALC665 codec support layout-id 12, 13 by insanelydeepak
- Added ALC292 codec support layout-id 12
- Added ALC283 layout-id 11 for Aspire V3-371 by Slbomber
- Added IDT92HD75B3X5 layout-id 11 with a custom pinconfig by olderst
- Prohibited kext unloading in policy mode since our policy is registered as static and cannot be unregistered
- Bumped module versions to 10.8 to avoid accidental loading on 10.7
- Added No-sleep, no-click patches into ALC892 info.plist
- Added ALC670 codec support layout-id 12 by Alex Auditore
- Added revision-id in ALC889 info.plist
- Added ALC260 codec support layout-id 12
- Added Creative CA0132 codec support layout-id 11 and 12
- Added CirrusLogic CS4213 codec support layout-id 28 by insanelydeepak
- Added revision 0x100202 into ALC888 info.plist
- Changed not working layout12 and platforms12 for Creative CA0132

#### v1.0.9
- Replace ALC888 patch 11d4198b->10ec0885 it prevents Sound Asserions on layout-id 1, 2, 3
- Deleted unnecessary ALC888 patch 11d41983
- Changed patch for Intel HD4600 HDMI (ig-platform-id 0x0d220003)
- Changed ALC275 Resources
- Deleted not working Intel HD4000 LVDS, HDMI (ig-platform-id 0x0166000A) Laptop only patch
- Added Conexant CX20583 codec support layout-id 3 by Mirone
- Added Conexant CX20585 codec support layout-id 3 by Mirone
- Added Conexant CX20588 codec support layout-id 3 by Mirone
- Added Conexant CX20724 codec support layout-id 3 by Mirone
- Added Conexant CX20752 codec support layout-id 3 by Mirone
- Added Conexant CX20756 codec support layout-id 3 by Mirone
- Added Conexant CX20757 codec support layout-id 3 by Mirone
- Added NoSleep/NoClick pathes to Conexant CX20590 info.plist
- Added ALC280 layout-id 4 Combo Jack Support by Mirone
- Added ALC1150 support for OS X 10.8
- Changed Comment lines for Toleda resources in 1150 info.plist
- Added IDT92HD93BXX codec support layout-id 12

#### v1.0.8
- Added ALC885 Toleda resources layout-id 1
- Added ALC887 Toleda resources layout-id 1, 2, 3
- Fix mistake in CX20751/2 info.plist and change layout-id to 28
- Added ALC888 Toleda resources layout-id 1, 2, 3
- Added ALC668 layout 20 for Asus G551JM 
- Added ALC275 layout-id 3 Mirone resources
- Added ALC283 layout-id 3 Mirone resources
- Added ALC284 layout-id 3 Mirone resources
- Added ALC288 layout-id 3 Mirone resources
- Added ALC290 layout-id 3 Mirone resources
- Added IDT92HD66C3/65 layout-id 3 Mirone resources
- Added IDT92HD71B7X layout-id 3 Mirone resources
- Added IDT92HD75B2X5 layout-id 3 Mirone resources
- Added IDT92HD75B3X5 layout-id 3 Mirone resources
- Added IDT92HD99BXX layout-id 3 Mirone resources
- Added IDT92HD87B1 layout-id 3 Mirone resources
- Change IDT92HD81B1X5 to Mirone resources layout-id 3
- Change IDT92HD91BXX  to Mirone resources layout-id 3
- AppleALC should not abort loading if it fails to find some kext

#### v1.0.7
- Added 0x100004 revision to ALC269
- Added custom ALC269 resources for SONY VAIO VPCEB3M1R layout-id 20
- Added custom ALC282 resources for Acer Aspire E1-572G layout-id 28
- Added into ALC269 info.plist no-sleep, no-click, headphone detect patches
- Added ALC282 resources for Acer Aspire on IvyBridge by Andrey1970 layout-id 27
- Added Mirone resources to ALC892 thanks to cecekpawon
- Added ALC668 resources for DELL Precision M3800 by Syscl
- Allowed providing non-existent layouts
- Added IDT92HD81B1X5 codec support layout-id 12 by RehabMan
- Added IDT92HD87B1/3 codec support layout-id 12 by RehabMan
- Added IDT92HD87B2/4 codec support layout-id 12 by RehabMan
- Added IDT92HD91BXX codec support layout-id 12 by RehabMan
- Added IDT92HD95 codec support layout-id 12 by RehabMan
- Added Conexant CX20751/2 codec support layout-id 12 by RehabMan
- Added ALC280 codec support layout-id 3 by Mirone
- Fixed CX20590 platform12.xml.zlib > platforms12.xml.zlib
- Reduced kext size by optimizing every platforms.xml.zlib in Resources folder
- Fixed codec-id in PinConfigs info.plist for IDT 92HD87B1/3 should be 287143633

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
