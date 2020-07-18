# AppleALC

[![Build Status](https://travis-ci.com/acidanthera/AppleALC.svg?branch=master)](https://travis-ci.com/acidanthera/AppleALC) [![Scan Status](https://scan.coverity.com/projects/16166/badge.svg?flat=1)](https://scan.coverity.com/projects/16166)

An open source kernel extension enabling native macOS HD audio for not officially supported codecs without any filesystem modifications.

#### Features

- Digital and analog audio support starting from the OS installation
- Recovery HD/macOS Installer audio support
- Automated codec detection
- Unsupported audio controller enabling (internal and external)
- Arbitrary kext patching
- Custom platform/layout injection
- Works with SIP / El Capitan
- Currently compatible with 10.8-10.16

#### Boot arguments

- `-alcdbg` to enable debug printing (available in DEBUG binaries).
- `-alcoff` to disable AppleALC.
- `-alcbeta` to enable AppleALC on unsupported macOS versions.
- `alcid=layout` to set layout-id value.
- `alc-delay=X` and (`alcdelay` property) to delay AppleHDAController startup in milliseconds.
- `-alcdhost` to enable custom audio drivers on 10.12+ (eg. Audirvana)
- `-alcaaplid=X` and (`apple-layout-id` property) to pass overridden layout-id.
- `alctsel=1` to change guarded TCSEL to TC0.

#### Support and discussion

[InsanelyMac](http://www.insanelymac.com/forum/topic/311293-applealc-—-dynamic-applehda-patching/) for English  
[AppleLife](https://applelife.ru/threads/applealc-dinamicheskij-patching-applehda.1171672/) for Russian

#### Contribution

To support more audio codecs in the binary packages you are asked to submit your configurations. Please read the [Wiki](https://github.com/acidanthera/AppleALC/wiki) for more details. For the contributors with programming skills the headers are filled with AppleDOC comments.

#### Credits

- [Apple](https://www.apple.com) for macOS  
- [Onyx The Black Cat](https://github.com/gdbinit/onyx-the-black-cat) by [fG!](https://reverse.put.as) for the base of the kernel patcher  
- [capstone](https://github.com/aquynh/capstone) by [Nguyen Anh Quynh](https://github.com/aquynh) for the disassembler module  
- [toleda](https://github.com/toleda), [Mirone](https://github.com/Mirone) and certain others for audio patches and layouts
- [Pike R. Alpha](https://github.com/Piker-Alpha) for [lzvn](https://github.com/Piker-Alpha/LZVN) decompression and certain HDMI patches  
- [07151129](https://github.com/07151129) for some code parts and suggestions  
- [vit9696](https://github.com/vit9696) for writing the software and maintaining it
- [vandroiy2013](https://github.com/vandroiy2013) for maintaining the codec database.
