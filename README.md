# AppleALC

[![Build Status](https://github.com/acidanthera/AppleALC/workflows/CI/badge.svg?branch=master)](https://github.com/acidanthera/AppleALC/actions) [![Scan Status](https://scan.coverity.com/projects/16166/badge.svg?flat=1)](https://scan.coverity.com/projects/16166)

AppleALC is an open-source kernel extension that enables native macOS HD audio for unsupported codecs without requiring any filesystem modifications. AppleALC can also be used for systems with digital-only audio.

Available languages:  
- English (Current)  
- [简体中文](https://github.com/acidanthera/AppleALC/blob/master/README_CN.md)

## Features
- **Digital and analog audio support** from the OS installation.
- **Recovery HD/macOS Installer audio support.**
- **Automated codec detection.**
- **Enabling of unsupported audio controllers**, both internal and external.
- **Arbitrary kext patching.**
- **Custom platform/layout injection.**
- **Compatibility with SIP/El Capitan+.**
- **Works with macOS versions** from 10.4 to 14.

## Credits
- **[Apple](https://www.apple.com)** for macOS.
- **[Onyx The Black Cat](https://github.com/gdbinit/onyx-the-black-cat)** by [fG!](https://reverse.put.as) for the kernel patcher base.
- **[capstone](https://github.com/aquynh/capstone)** by [Nguyen Anh Quynh](https://github.com/aquynh) for the disassembler module.
- **[toleda](https://github.com/toleda)**, **[Mirone](https://github.com/Mirone)**, and others for audio patches and layouts.
- **[Pike R. Alpha](https://github.com/Piker-Alpha)** for [lzvn](https://github.com/Piker-Alpha/LZVN) decompression and HDMI patches.
- **[07151129](https://github.com/07151129)** for code contributions and suggestions.
- **[roddy20](https://github.com/roddy20)** for training and research on codec patching.
- **[vit9696](https://github.com/vit9696)** for writing and maintaining the software.
- **[Andrey1970AppleLife](https://github.com/Andrey1970AppleLife)** and **[vandroiy2013](https://github.com/vandroiy2013)** for maintaining the codec database.

## Installation
Detailed installation instructions are available on the [wiki](https://github.com/acidanthera/AppleALC/wiki).  
Prebuilt binaries can be found on the [releases](https://github.com/acidanthera/AppleALC/releases) page.

## Contribution
To support additional audio codecs in the binary packages, we encourage you to submit your configurations. For more details, please refer to the [wiki](https://github.com/acidanthera/AppleALC/wiki). Contributors with programming skills will find headers filled with AppleDOC comments.

## Support and Discussion
- [InsanelyMac topic](http://www.insanelymac.com/forum/topic/311293-applealc-—-dynamic-applehda-patching/) (English)
- [AppleLife topic](https://applelife.ru/threads/applealc-dinamicheskij-patching-applehda.1171672/) (Russian)

## Donations
Writing and maintaining code is rewarding but time-consuming. If you appreciate the work, consider contributing, reporting bugs, or providing support to other users.
