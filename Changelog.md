AppleALC Changelog
==================
#### v1.6.8
- Replace patch for 500 Series(0x43C8) PCH HD Audio

#### v1.6.7
- Added 600-series controller patch by @R-a-s-c-a-l
- Added ALC282 layout-id 69 for Lenovo IdeaPad Z510 by hoseinrez
- Added ALC285 layout-id 66 for for Lenovo Legion S740 15-IRH by @R-a-s-c-a-l
- Fix PinConfigs Device and Port in ALC662v3 by static-host
- Added ALC269 layout-id 25 for Medium Akoya p6653 by hua0512
- Added ALC235 layout-id 13 for Deskmini H470 by dumk1217
- Added ALC283 layout-id 12 for ThinkCentre M73(10AX) ALC283 by dumk1217
- Added ALC285 layout-id 88 for Yoga S740 by frozenzero123
- Fix ALC256 layout-id 67 unable to change the built-in Speaker Volume through the hotkey after unplugging the headphone by @R-a-s-c-a-l
- Fix ALC298 layout-id 11 wake data for Alienware 17 R4 by RockJesus
- Added ALC282 layout-id 30 for Soarsea S210H by Jokerman1991
- Added ALC662 layout-id 19 for MSI X79A-GD65 by @wy414012

#### v1.6.6
- Added ALC256 layout-id 24 for Intel NUC NUC10i5FNH by Andres ZeroCross
- Added Conexant CX11970 (CX8400) layout-id 13 for Acer Swift 3 SF31* (Ice Lake) by m0d16l14n1
- Added ALCS1200A layout-id 7 for B550M Gaming Carbon WIFI by Kila2
- Try to solve wake up mute for GP75 9SD by Win7GM
- Added ALC256 layout-id 33 for Huawei Matebook D15 MRC-W10 by im1ke
- Added ALC892 layout-id 23 for ASRock B365 Pro4 by TheHackGuy
- Fixed ALC221 layout-id 11 for HP6300/8300 rear line-in jack by adding DSP functions by aloha-cn

#### v1.6.5
- Added ALC236 for Lenovo IdeaPad 330S-14IKB by Ab2774
- Some updates and revisions for ALC236 LayoutID 36 by volcbs
- Added ALC897 for GIGABYTE Z590 Gaming X by Sergey_Galan 
- Added ALC294 layout-id 99 for ASUS ROG GU502LV by htmambo
- Added ALC256 layout-id 99 for XiaoMiPro 2020 by htmambo
- Added ALC897 layout-id 23 for Chuwi-CoreBookX14 by weachy
- Added ALC256 layout-id 20 for Asus AIO PC V222UAK-WA541T by andreszerocross
- Added ALC236 layout-id 23 for Lenovo Air 13 IML(S530-13IML) by JudySL

#### v1.6.4
- Added 10.4 and 10.5 support, and 10.6 and 10.7 support in 32-bit mode
- Update STAC9200 layout-id 11 to support 10.4 to 10.6
- Added STAC9205 layout-id 11 for Dell Inspiron 1520 and Latitude D630
- Fixed crash on GPUs without digital audio support introduced in 1.6.1
- Added ALC623 layout-id 21 for Lenovo M70T by Andres ZeroCross
- Seperated Laptop/Desktop patches for 8086:A171 to fix HDMI audio on Intel NUC
- Added ALC236 layout-id 36 for Lenovo Ideapad 510s 14isk by volcbs
- Added ALC235 layout-id 12 for Dell Optiplex 7040 MT by wern-apfel
- Improved CA0132 layout-id 7 by removing not needed MuteGPIO by wern-apfel
- Added ALC235 layout-id 8 for Intel NUC 8 by wern-apfel
- Added ALC269 layout-id 69 for MSI GF63 Thin 9SEXR  by Vorshim92
- Added ALC289 layout-id 93 for XPS 9500 4k by sweet3c
- Added ALC892 layout-id 32 for custom G4/G5mod

#### v1.6.3
- Update ALC289 layout-id 87 by GitNaufal
- Update ALC235 layout-id 18 for AsRock 310 BB by viorel78

#### v1.6.2
- Added constants for macOS 12 support
- Added 10.6 and 10.7 support in 64-bit mode
- Fixed HDMI Audio 0xA171 for Acer VX5-591G by dongcodebmt
- Added ALC256 layout-id 70 for Hasee KingBook X57S1 by m0d16l14n1
- Update WakeConfigData for ALC255 layout-id 96 by wozhinengpa

#### v1.6.1
- Fixed broken data in CS4206's layout76.xml
- Added PathMapID 4206 and 8800 for ALC885's Layout 67 and 73
- Fixed broken data in ALC289's layout87.xml
- Fixed automatic resource formatting on build
- Fixed ALC885's Info.plist Platforms entry
- Added 400 Series (0xF1C8 Z490 + Intel 11 Gen) PCH HD Audio Controller
- Fixed `alc-verb` device indexing giving varying results over reboots
- Added device listing with indices via `-L`/`-l` in `alc-verb`
- Added AppleALCU kext variant for digital-only audio setup
- Disabled kext patching for verb support and delays when not requested
- FIxed replace count in `WhiskeyLake` HDA patches
- Separated Intel `WhiskeyLake` laptop and desktop patches
- Added ALC1220 layout-id 35 for MSI GP75 9SD by Win7GM
- Added ALC1200 (display as ALCS1200A) layout-id 69 for Asrock Z490M ITX/AC by Lorys89 & Vorshim92
- Added ALC293 layout-id 30 for HASEE ZX8-CT5DA by RushiaBoingBoing
- Added ALC255 layout-id 96 for dell 5559 by Bhavin
- Fix mute on Conxexant CX 20724 by Human7900
- Added ALC1220 layout-id 17 for Gigabyte Z490 Vision G manual SP/HP by NIBLIZE
- Added ALC255 layout-id 82 for minisforum U820 by daliansky
- Added ALC282 layout-id 21 for TinyMonster ECO by DalianSky

#### v1.6.0
- Added `use-layout-id` property to use `layout-id` as is on Macs
- Added `use-apple-layout-id` property to use `apple-layout-id` as `layout-id` on Macs
- Fixed CS4206 and ALC885 support for legacy Macs
- Improvement ALC289 layout-id 87 for Alienware m15 by GitNaufal
- Added ALC1220 layout-id 98 for Mi Gaming Notebook Creator by Xsixu
- Added ALC1220 layout-id 100 for Hasee_G8-CU7PK by R-a-s-c-a-l

#### v1.5.9
- Added CS4206 layout-id 24 and 60
- Added 500 Series (0xF0C8 Z590 + Intel 10 Gen) PCH HD Audio Controller
- Added 500 Series (0x43C8 Z590 + Intel 11 Gen) PCH HD Audio Controller
- Added ALC289 layout-id 87 for Alienware m15 by GitNaufal
- Added ALC289 layout-id 15 for Dell 7730 Precision CM240 by MacPeet
- Added ALC897 layout-id 69 for MSI-Z490-A Pro by mathcampbell
- Added IDT 92HD95 layout-id 14 for LenovoG710 by Svilen88
- Added ALC235 layout-id 18 for asrock 310 bb by viorel78
- Added controller patch for 100 Series (8086:A170) by dhinakg

#### v1.5.8
- Improved resource packaging by stripping optional tags
- General enhancement & improvement for ALC230 by Ab2774
- Added ALC274 layout-id 35 by jackjack1-su

#### v1.5.7
- Add support for legacy Macs
- Added ALC289 layout-id 99 for Dell XPS 13 9300 by DalianSky
- Added ALC225 layout-id 90 for Dell Inspiron 5379 by fast900
- Added ALC274 layout-id 28 working speakers/mic Maingear Element 3 (TongFang 17 Barebone) by 343iChurch
- Added ALC256 layout-id 77 for Asus x430_s4300FN by fangf2018
- Added ALC256 layout-id 88 for Asus x430_s4300FN by fangf2018
- Fix Mic for ALC221 layout-id 88 HP ProDesk 400 G2 Desktop Mini PC
- Added ALC897 layout-id 66 for ASUS_PRIME_B460M-K by Dynamix1997

#### v1.5.6
- Improved `alc-verbs` availability checking
- Add ALC256 layout-id 67 for Dell OptiPlex 7080
- Add ALC222 layout-id 11 for HP EliteDesk 800 G6 Mini
- Add ALC256 layout-id 69 for Xiaomi Pro Enhanced 2019
- Add ALC235 layout-id 37 for Lenovo IdeaCentre Mini 5
- CX20751_2 fixed mic gain slider in Big Sur

#### v1.5.5
- Fixed kext loading issues on 10.8 and similar operating system
- Added a requirement to inject `alc-verbs` or use `alcverbs=1` argument for custom verbs
- Added ALC255 layout-id 66 for Dell Optiplex7060/7070MT(Separate LineOut) by Dynamix1997
- Fixed Jack Sense and EAPD on ALC 236 layout ID 14 by erinviegas
- Added VIA VT2021 layout-id 13 support for all 3 analog lineOUTs on Gigabyte GA-Z77X-D3H (rev. 1.0) by enrysan0
- Added ALC283 layout-id 13 for (Alldo)Cube Mix Plus by Aldo97
- Fix PinConfigs ALC662v3 for Lenovo M415-D339
- Added ALC295 layout-id 22 for HP Spectre x360 by aleixjf
- Added ALC285 layout-id 71 for Spectre x360 ap0xxx by jpuxdev
- Added ALC221 layout-id 88 for HP ProDesk 400 G2 Desktop Mini PC by dragonbbc
- Added CX8200 layout-id 80 for LG Gram 17 17z990 by rdmitry0911
- Added ALC269 layout-id 91 for Chuwi CoreBox by Luca1991
- Modify CX20632 layout-id 20 - Added Mic support and outputs mute controls - HP EliteDesk 800 G4/G5 mini by sisumara
- Added ALC671 layout-id 16 for Fujitsu Q558 by sisumara
- Added `-dev` option to alc-verb to support sending commands to all codecs

#### v1.5.4
- Improved Ice Lake controller patches by fewtarius
- Added verb sending functionality from userspace by black-dragon74
- Added ALC235 (display as ALC233) layout-id 35 for Lenovo Qitian M420-D046(C) by crysehillmes
- Added ALC892 layout-id 100 for MSI Z370-A PRO by GeorgeWan
- Added ALCS1200A layout-id 51 for for ASROCK Z490 Steel Legend by GeorgeWan
- Added ALC662 layout-id 66 for Lenovo Qitian M415-D339 by static-host
- Fixed ALC285 layout-Id 21 for X1C6 (by @fewtarius)
- Added ALC272 layout-id 12 for Lenovo Y470 by amu_1680c
- Added patch CX20751/2 by vasishath to fix internal mic gain adjustment (this fix microphone volume slider in system preferences)
- Added ALC230 layout 13 & 20 Jack Sense and EAPD support and add WakeConfigData to layout 13
- Added ALC290 layout-id 10 for HP Envy 15t-k200 w/ Beats Audio 2.1 by temp1122-sys

#### v1.5.3
- Fix ALCS1200A lost ID 11 by owen0o0
- Added MacKernelSDK with Xcode 12 compatibility
- Added CX20632 layout-id 20 for HP Elitedesk 800 G5 Mini by phucnguyen2411
- Added ALC269VC layout-id 24 for Hasee U45S1 by zero816
- Added ALC283 layout-id 45 for NUC7 by micsell
- Added ALC235 layout-id 99 for Lenovo TianYi 510s Mini by DalianSky
- Added ALC1150 layout-id 99 for Gigabyte GA-Z97X-UD5H by DalianSky
- Correct layout 13 for IDT92HD87B2/4

#### v1.5.2
- Added missing layout7.xml for CA0132
- Added 400 series 0xA3F0 controller patch by goomadao
- Added ALCS1200A layout-id 11 for MAG-Z490-TOMAHAWK by owen0o0
- Added ALC269 layout-id 128 for Laptop NS4SL01 by ryahpalma
- Added ALCS1200A layout-id 50 for Gigabyte B460M Aorus Pro by VanXNF
- Added ALC280 layout-id 17 for Dell Optiplex 9020 SFF by pkendall64
- Added ALC236 layout-id 14 for Lenovo 330S by erinviegas
- Added ALC887 layout-id 12 for ASUS H81M-D by VanXNF
- Added ALCS1200A layout-id 49 for Asrock Z490M-ITX by VanXNF
- Added ALC269 layout-id 23 for Thinkpad T430 with fixed micophone inputlevel by haotiangood
- Added ALC245 layout-is 11 and 12 for Lenovo by soto2080
- Added ALC245 layout-id 13 for HP Omen 15 2020 by lunjielee
- Added ALC287 layout-id 11 for HP Omen 15 2020 by lunjielee

#### v1.5.1
- Set MinKernel Catalina for 400 Series
- Added constants for 11.0 support
- Added 400 series 0x6c8 and 0x2c8 controller patch (thanks @lvs1974)
- Added ALC295 layout-id 24 for HP Pavilion / OMEN-2 by zty199
- Update CA0132 layout-id 7, 4, 2, 1 by wern-apfel
- Added IDT92HD95 layout-id 11 by wern-apfel
- Added Icelake Smart Sound Technology Audio Controller
- Fix Headphone NodeID，Add SPDIFOut NodeID，ALC255 layout-id 20 for DELL 7447 by was3912734
- Added ALC290 layout-id 15 for HP m6 n015dx by MacPeet
- Added ALC285 layout-61 for Lenovo Yoga C740 by fewtarius
- Added CA0132 LayoutID 99 - 5.1 channel for Alienware-M17X-R4 by DalianSky
- Added ALC289 layout-id 11 for alienware m17r2 by leeoem

#### v1.5.0
- Update ALC283 layout-id 88 by xiaoleGun
- Fixed accidental reading of `alc-layout-id` on non-Apple firmwares
- Add patch to fix internal mic gain adjustment Conexant CX8050
- Move ALC255 layout-id 7 to layout-id 86
- Added ALC257 layout-id 86 for Lenovo T480 by armenio
- Fixed can't activate mute problem Conexant CX8070 layout-id 15 by lietxia
- Added ALC255 layout-id 20 for DELL 7447 by was3912734. Add Subwoofer drive.
- Added ALC662 layout-id 18 for MP67-DI/ESPRIMO Q900 by ryahpalma
- Added ALC256 layout-id 19 for Matebook X Pro 2019 by Durian-Life
- Added ALC256 layout-id 76 (4CH) for Matebook X Pro 2019 by Durian-Life

#### v1.4.9
- Added IDT92HD73E1X5 layout-id 15 for HP Envy h8 1425eg by MacPeet
- Fix Headphone NodeID ALC269VC layout-id 22 Veritron Z4640G by Andres ZeroCross
- Added ALC255 layout-id 29 for Acer Predator G3-571 by dhinakg
- Added ALC256 layout-id 17 for Magicbook with 4CH by hjmmc
- Updated ConfigData & Added WakeConfigData for ALC295 layout-id 14 by hieplpvip
- Added ALC671 layout-id 88 for HP 280 Pro G4 by lcp6668
- Added ALC294 layout-id 22 for ASUS ROG GL504GW by cowpod
- Added ALC892 layout-id 22 for ASRock Z390m-ITX/ac by imEgo
- Added wakeconfigdata for ALC230 layout-id 20 by ameenjuz
- Added ALC235 layout-id 24 for ASUS TUF FX705GM by therealgudv1n
- Added ALC892 layout-id 20 for GIGABYTE Z390M GAMING by bokeylee
- Added ALC298 layout-id 16 for Dell Precision 5520 by Pingren
- Added ALC269 layout-id 77 for Thunderobot-G7000S by Phoenix-Liang
- Rename 400 series LP controller, add 400 series PCH-H controller

#### v1.4.8
- MaxKernel HS for GM/GP
- Support startup delay for AppleHDAController via `alc-delay` property or `alcdelay` boot-arg (in ms)
- Guarded TCSEL change to TC0 with `alctsel=<01 00 00 00>` property or `alctsel=1` boot-arg
- Improved performance with Lilu 1.4.3 APIs
- Added ALC257 layout-id 99 and100 for Lenovo XiaoXin Pro 2019 by DalianSky
- Added ALC283 layout-id 88 for DELL R14 3437 by Zoran
- Add mute button to ALC230 layout 13 by Ab2774
- Update for Better Input Range ALC 235 by Andres ZeroCross
- Added ALC269VC layout-id 22 for Acer Veriton Z4640G by Andres ZeroCross
- Updated ConfigData & Added WakeConfigData to fix CodecCommander dependency CX20751/2 layout-id 28 LineIn Sniki
- Updated ConfigData & Updated WakeConfigData to fix CodecCommander dependency ALC221 layout-id 11 LineIn(s) by Sniki
- Added ALC255/ALC3234 layout-id 7 for Dell 7348 by armenio
- Fix Conexant CX8050 LineIn for headsets with combo jack

#### v1.4.7
- Added support for Intel C620 series PCH Audio
- Fixed injected MaximumBootBeepVolume
- Added ALC256 layout-id 23 by Andres ZeroCross for Razer Blade 15 RZ09-02705E75
- Added mute button to ALC230 layout 20
- Added ALC295 layout-id 23 for HP OMEN 15-AX000 by Lancet-z
- Added ALC256 layout-id 16 for Dell 7000 Series with 2.1Ch by vicjl
- Added ALC294 layout-id 66 for ASUS X542UQR by KKKIIINNN
- Added ALC236 layout-id 12 for Lenovo Xiaoxin Air 14IKBR by AlexanderLake
- Added ALC292 layout-id 15 for LENOVO THINKPAD T450_T450s_X240 - Manual by ModeMacPeet
- Added ALC1220 layout-id 30 for Z370 AORUS Gaming 7 - Output SP/HP SwitchMode by MacPeet
- Added ALC294 layout-id 28 for Asus ROG G531GD by ayatkyo
- Added ALC255 layout-id 71 for Acer Aspire A515-51(G) by DoctorStrange96
- Added ALC235 layout-id 17 for Lenovo ThinkCentre Tiny M720q by marianvlaza

#### v1.4.6
- Add missing pinconfig ALC235 layout-id 16
- Fixed speaker/headphone priority ALC700 layout-id 11
- 300 Series PCH HD Audio is native in 10.14+
- Added ALC215 layout-id 18 for HP 830 G6 by 965987400abc
- Added TCSEL change to TC0 for Intel codecs

#### v1.4.5
- Update MuteGPIO and AMP Values ALC255 layout-id 21 by Andres ZeroCross
- Added ALC272 layout-id 21 for  Lenovo All In One PC C440 by Andres ZeroCross
- Conflict patches for NUC7 HDMI audio are deleted.
- Added ALC285 layout-id 52 for Thinkpad P52 by liuyishengalan
- Added  Intel 400 series controller (Comet Lake) by fewtarius
- Added 4 channel support and fixed mute button to ALC256 layout 97
- Added ALC1220 layout-id 99 for MiBook 2019 by Dynamix1997
- Added ALC262 layout-id 66 for MS-7847 by Dynamix1997
- Added ALC269VC layout-id 47 for Hasee K790s by Dynamix1997

#### v1.4.4
- Added ALC269 layout-id 12 for Asus Vivobook S200CE by chunnann
- Update ALC269 layout-id 19 for Asus Vivobook S300CA by chunnann
- Update ALC230 layout-id 20 to fix sleep after wake by ameenjuz
- Added CX8150 layout-id 22 for ASUS VivoBook S405UA-EB906T by Andres ZeroCross
- Added ALC285 layout-id 31 for Thinkpad X1E 8750H version by Flymin

#### v1.4.3
- Added support for `No-hda-gfx` HDEF property disabling IGPU digital audio
- Added CX8050 layout-id 13 for ASUS S410U/X411U by cowpod
- Fix layout 13 CX8050 microphone after sleep by cowpod
- Added STAC9872AK layout-id 12 for Sony VGN-FZ11MR by ctich
- Added support for intel NUC7 HDMI audio device-id 719d by Sevift
- Update configdata for dell-5488 by daggeryu
- Fixed ALC292 layout-id 55 with linein (dock mic) boost issue by baesar0

#### v1.4.2
- Added Conexant CX20632 layout-id 28 for HP EliteDesk by Halelluja12
- Added wake verb for ALC1220 layout-id 34 for MSI P65 Creator / MSI GS65
- Added Conexant CX20632 layout 23 for  HP ProDesk 480 G4 by frankiezdh
- Fix headphone and speaker switch not working for CX8200 HP Probook 440 G5
- Added ALC1220 layout-id 21 for MSI GE63 Raider RGB 8RF by Andres ZeroCross
- Added ALC292 layout-id 32 for Lenovo T450s by EchoEsprit
- Fixed string Platforms97.xml.zlib Realtek ALC256 layout-id 97 by DalianSky
- Fix ALC700 HeadPhones and SPDIF outputs by osy86

#### v1.4.1
- Fixed memory leaks
- Added AMD Radeon R9 Fury/R9 Fury X HDMI audio support
- Added ALC256 layout-id 66 for ASUS Y5000U X507UBR by lgs3137
- Added ALC235 layout-id 22 for Asus ROG GL553VD-FY380 by Andres ZeroCross
- Added ALC892 layout-id 90 for GIGABYTE B360M M AORUS PRO by xunyard
- Added ALC298 layout-id 32 for XPS 9570 8750H version by smallssnow
- Added ALC292 layout-id 55 for Dell Latitude E6440/E6540 with docking station linein/lineout support by baesar0
- Added ALC256 layout-id 97 for MateBook X Pro 2019 by DalianSky
- Added ALC269 layout-id 100 for GPD P2 Max by DalianSky
- Merged Lenovo Air13 IWL with WhiskeyLake Mobile PCH HD Audio
- Added CX8200 layout-id 23 for HP Probook 440 G5 by frankiezdh

#### v1.4.0
- Unified release archive names
- Added AMD Zen 2 controller patches by camielverdult
- Added ALC274 layout-id 21 for Dell Inspiron 27-7777 AIO Series by Andres ZeroCross
- Added ALC257 layout-id 18 for Lenovo Legion Y540 and Y7000-2019 by xiaoMGitHub
- Added ALC289 layout-id 23 for Acer PT515-51 By Bugprogrammer and Rover
- Added ALC285 layout 31 for Thinkpad X1E 8750H version by Flymin

#### v1.3.9
- Allow loading on 10.15 without `-lilubetaall`
- Added ALC272 layout-id 18 for Lenovo B570 and B570e by Sniki
- Added Conexant CX8070 (CX11880) layout-id 15 for Lenovo ThinkPad E590 by MacPeet
- Added ALC1220 layout-id 29 for Z390 Aorus Ultra - Output SP/HP SwitchMode by MacPeet
- Added Conexant CX8200 layout-id 15 for HP ZbooK 15UG4 by MacPeet
- Update AD1984 layout-id 11 by MacPeet
- Added ALC663 layout-id 15 for Fujitsu Celsius r670 by MacPeet
- Added ALC700 layout-id 11 for NUC HC
- Added AMD Vega-M HDMI
- Added ALC235 layout-id 12 for Lenovo Rescuer 15ISK by Z39
- Added ALC235 layout-id 21 for Lenovo C530 Desktop PC by Andres ZeroCross
- Added ALC256 layout-id 22 for Asus VivoBook Pro 17 N705UDR by Andres ZeroCross
- Added ALC255(3234) layout-id 11 for dell optiplex by Heporis
- Added ALC256 layout-id 14 for Dell Series with Subwoofer courtesy of @insanelydeepak
- Update ALC892 layout-id 97 by gitawake
- Update ALC298 layout-id 66 for MECHREVO S1 by lgs3137
- Added ALCS1220A layout-id 11 for Asus Z270-G (based on Mirone's layout 7) by Kushamot 

#### v1.3.8
- Fixed rare random audio init failure on 10.14
- Added ALC255 (3246) layout-id 100 for alienware alpha r2 by DalianSky
- Added ALC262 layout-id 7 for MS-7480N1 by DalianSky
- Added ALC236 layout-id 54 for DELL-5488 by Daggeryu
- Added ALC280 layout-id 16 for Optiplex 9020SFF by cowpod
- Added ALC1220 layout-id 34 for MSI P65 Creator Laptop by CleverCoder
- Added ALC892 layout-id 97 for Hasee K770e i7 D1 (P170sm-a) by gitawake
- Fix Typo/Update Pinconfig ALC294 layout21 by Andres and Add HDAController patch for WhiskeyLake Notebook
- Update PathMap for ALC298 layout-id 99 for XiaoMi-Pro
- Added ALC892 layout-id 17 for MSI B150M MORTAR - SwitchMode by MacPeet
- Added ALC892 layout-id 18 for MSI B150M MORTAR - ManualMode by MacPeet
- Added ALC1220 layout-id 28 for Z390 Aorus Ultra - Output SP/HP Manualmode by MacPeet
- Added VT1802 layout-id 65 for hasee K650D by ivothgle
- Added ALCS1220A layout-id 20 and 21 for Asus ROG STRIX Z370-F Gaming by RodionS & Nacho
- Added ALC270 layout-id 21 for Asus A46CB-WX024D Laptop by Andres ZeroCross

#### v.1.3.7
- VT2020_2021 anticlick patches by chrome
- VT1802 anticlick patches by chrome
- VT1705 anticlick patches by chrome
- AD2000B anticlick patches by chrome
- AD1988B anticlick patches by chrome
- CA0132 added sоme anticlick patches by chrome
- STAC9200 10.14.4 compatible anticlick patch by chrome
- ALCS1200A 10.14.4 compatible anticlick patch by chrome
- ALC891 10.14.4 compatible anticlick patch by chrome
- ALC1150 10.14.4 compatible anticlick patch by chrome
- Update resources for ALC298 layout-id 99 for XiaoMi Pro by stevezhengshiqi
- Added ALC235 layout-id 16 for ASUS GL553VD by MacPeet  
- Added ALC256 layout-id 57 for huawei honor magic book by ROSstudy

#### v1.3.6
- Update Revision 0x100001 for CX20751/2 (Toshiba Kira 107)
- Added ALC255 layout-id 15 for Gigabyte Brix BRi5(H), BRi7(H)] by najdanovicivan
- Added ALC295/ALC3254 for Dell Latitude 5290 by Unbelievable9
- Fix ALC1220 audio after sleep
- Added ALC1220 layout-id 27 for Gigabyte-Z370-HD3P by lostwolf
- Added ALC299 layout-id 22 for Dell XPS 13 by andreszerocross
- Added ALC887-VD layout-id 87 for GA-Z97-HD3 ver2.1 by varrtix
- Update resources for ALC236 layout-id 18 by xiaoMGitHub
- Added CX20753/4 layout-id 15 for Lenovo Thinkpad E580 by MacPeet
- Added CX20753/4 layout-id 14 by InsanelyDeepak
- Change ALC887 layout-id 52 for Asus PRIME Z270-P by ctich
- Added ALC887 layout-id 53 for Asus PRIME Z270-P by ctich

#### v1.3.5
- Added AD1988A layout-id 12 by chrome
- Added ALC282 layout-id 41/43/51 for Lenovo Y410P/Y430P/Y510P by yunsur
- Added CX8400 layout-id 12 for Elitebook 1050 G1 by tyufhl & kuque
- Added ALC269VC layout-id 55 for Thinkpad X230 with Dock4338
- Added ALC235 layout-id 15 for ASUS FX53VD by qiuchenly
- Added Chinese readme
- Added ALC255 layout-id 31 for ASUS UX360CA by cowpod
- Added ALC269VC layout-id 188 for Lenovo Y500 by BaoStorm
- Update audio layouts with fixes ALC887, ALC892, ALC1220, ALCS1220A and ALC1150 by toleda
- Added ALC889 layout-id 2, 3 by toleda
- Added ALCS1200A layout-id 1, 2, 3 by toleda
- Added ALC256 layout-id 5  by chrome
- Fix headphone input in ALC255 layout-id 99 for XiaoMI Air by ourfor
- Added ALC285 layout-id 21 by andreszerocross
- Added ALC298 layout-id 22 by andreszerocross
- Added ALC299 layout-id 21 by andreszerocross
- Added CX20753 layout-id 21 by andreszerocross
- Added ALC233 layout-id 21 by andreszerocross
- Added IDT 92HD73C1X5 layout-id 21 by andreszerocross
- Added ALC294 layout-id 21 by andreszerocross
- Added ALC256 layout-id 21 by andreszerocross
- Added CX8200 layout-id 21 by andreszerocross
- Added CX20751/2 layout-id 21 by andreszerocross
- Added ALC286 layout-id 11 for Lenovo Yoga3 pro by gdllzkusi
- Added ALC236 layout-id 18 for LEGION Y7000/Y530 by xiaoMGitHub
- Fixed ALC256 layout-id 56 by daliansky
- Added 10.14.4 compatible anticlick patch by chrome
- Added CX20757 layout-id 28 for Lenovo G510 by Z39
- Simplified analog codec code to permit special locations like VMware or AMD
- Added patches for AMD Zen controllers (0x1457, 0x15E3) thx Ydeng and AlGreyy
- Added ALC898 layout-id 66 for Clevo P750DM2-G by chrome
- Added ALC283 layout-id 15 for LENOVO IDEAPAD 14 by MacPeet
- Added ALC671 layout-id 15 for Fujitsu  Esprimo C720 by MacPeet
- Added ALC221 layout-id 15 for HP ELITE DESK 800 G1 by MacPeet
- Added ALC269 layout-id 21 for Dell OptiPlex 7010 by Goldfish64

#### v1.3.4
- Disabled AppleGFXHDA matching onto HDEF to force AppleHDA usage
- Added Wern Apfel's menubar patch for CX20590
- Added ALC255 layout-id 21 for Asus X441UA-WX096D by Andres ZeroCross
- Added ALC255 layout-id 21 for Asus VivoBook Pro 15 CX8150 by Andres ZeroCross
- Added ability to disable controller injection with property `no-controller-inject`
- Added SigmaTel STAC9200 layout-id 11 for Dell Precision 390 by Goldfish64
- Added support for HDMI Audio of Alienware Alpha GTX860M
- Added ALC1220 layout-id 15 for MSI WT75 Laptop by fleaplus.
- Fix ConfigData in  ALC256 (3246) for Dell Series by Insanelydeepak
- Added ALC298 layout-id 21 for Lenovo 720S-15IKB by Andres ZeroCross
- Added ALC236 layout-id 99 for Lenovo Air 13 IWL by DalianSky
- Fix speaker volume slider CX8050
- Enable NVIDIA HDMI patches for laptop GPUs
- Added ALC280 layout-id 21 for Dell Precision ﻿T7610 ﻿Workstation by Andres ZeroCross
- Added ALC295 layout-id 21 for Acer Nitro 5 Spin (NP515-51) by Andres ZeroCross
- Added VIA VT1705 layout-id 21 for ECS H81H3-M4 (1.0A) by Andres ZeroCross
- Added ALC269 layoutid=88 for MECHREVO X8Ti Plus by DalianSky
- Added ALC662 layout-id 17 for HP Compaq Elite 8000 SFF by aloha_cn
- Added ALC288 layout-id 23 for Dell XPS 9343 by yyfn
- Added ALC892 layout-id 16 for MSI GL73-8RD by MacPeet
- Change CX20585 layout-id 13 resources
- Added ALC887 layout-id 52 for Asus PRIME Z270-P by ctich
- Added AD1984A layout-id 44 by Giesteira
- Added ALC269 layout-id 44 for Samsung _NP350V5C by Giesteira
- Added ALC269 layout-id 17 by MacPeet
- Added ALC1220 layout-id 16 for Gigabyte Z390 by MacPeet

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
