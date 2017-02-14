修改增加支持 华擎 ASRock DeskMini 110(H110M-STX) 声卡（ALC283）。

layout-id 是 **66**。

必须安装[CodecCommander.kext](https://bitbucket.org/RehabMan/os-x-eapd-codec-commander/downloads)，否则无法正常发出声音。



Added ALC283 layout-id **66** for ASRock DeskMini 110(H110M-STX).  

[CodecCommander.kext](https://bitbucket.org/RehabMan/os-x-eapd-codec-commander/downloads) is nessesary, otherwise the audio may not work properly.  



------------------原项目信息------------------

AppleALC
========

An open source kernel extension enabling native macOS HD audio for not officially supported codecs without any filesystem modifications.

#### Features
- Audio support starting from the OS installation
- Recovery HD/macOS Installer audio support
- Automated codec detection
- Unsupported audio controller enabling
- Arbitrary kext patching
- Custom platform/layout injection
- Works with SIP / El Capitan
- Currently compatible with 10.8-10.12

#### Credits
- [Apple](https://www.apple.com) for macOS  
- [Onyx The Black Cat](https://github.com/gdbinit/onyx-the-black-cat) by [fG!](https://reverse.put.as) for the base of the kernel patcher  
- [capstone](https://github.com/aquynh/capstone) by [Nguyen Anh Quynh](https://github.com/aquynh) for the disassembler module  
- [toleda](https://github.com/toleda), [Mirone](https://github.com/Mirone) and certain others for audio patches and layouts
- [Pike R. Alpha](https://github.com/Piker-Alpha) for [lzvn](https://github.com/Piker-Alpha/LZVN) decompression and certain HDMI patches  
- [07151129](https://github.com/07151129) for some code parts and suggestions  
- [vit9696](https://github.com/vit9696) for writing the software and maintaining it
- [vandroiy2013](https://github.com/vandroiy2013) for maintaining the codec database

#### Installation
The minimal instruction is available on the [wiki](https://github.com/vit9696/AppleALC/wiki).  
The prebuilt binaries are available on [releases](https://github.com/vit9696/AppleALC/releases) page.

#### Contribution
To support more audio codecs in the binary packages you are asked to submit your configurations. Please read the [wiki](https://github.com/vit9696/AppleALC/wiki) for more details. For the contributors with programming skills the headers are filled with AppleDOC comments.

#### Support and discussion
[InsanelyMac topic](http://www.insanelymac.com/forum/topic/311293-applealc-—-dynamic-applehda-patching/) in English  
[AppleLife topic](https://applelife.ru/threads/applealc-dinamicheskij-patching-applehda.1171672/) in Russian

#### Donations
Writing and supporting code is fun but it takes time. If you want to thank the author for his work consider contributing, bugreporting, or providing the support to other users.
