AppleALC
========

[![Build Status](https://github.com/acidanthera/AppleALC/workflows/CI/badge.svg?branch=master)](https://github.com/acidanthera/AppleALC/actions) [![Scan Status](https://scan.coverity.com/projects/16166/badge.svg?flat=1)](https://scan.coverity.com/projects/16166)

一个开源的内核扩展，为非官方的声卡提供支持的 Codec，无需修改任何系统文件，即可启用原生的 macOS HD 音频，AppleALCU 可用于具有纯数字音频的系统。

[英文](https://github.com/acidanthera/AppleALC/blob/master/README.md)  
[简体中文](https://github.com/acidanthera/AppleALC/blob/master/README_CN.md)  

#### 特性
- 在系统安装阶段即可使用数字或模拟音频
- 支持恢复模式与安装模式
- 自动检测声卡 Codec
- 启用 Apple 官方不支持的编解码器 (无论是内置还是外置的声卡设备)
- 任意的 Kext 修改补丁
- 可自定义设备的 Layout 和 platform
- 工作在 SIP 模式 / El Capitan+ 系统
- 当前版本兼容: 10.4-13

#### 致谢
- [Apple](https://www.apple.com) 设计的 macOS 操作系统 
- [Onyx The Black Cat](https://github.com/gdbinit/onyx-the-black-cat) by [fG!](https://reverse.put.as) 提供了数据库和内核补丁
- [capstone](https://github.com/aquynh/capstone) by [Nguyen Anh Quynh](https://github.com/aquynh) 提供了部分反汇编程序模块 
- [toleda](https://github.com/toleda), [Mirone](https://github.com/Mirone) 以及某些其他的layout和音频补丁
- [Pike R. Alpha](https://github.com/Piker-Alpha) for [lzvn](https://github.com/Piker-Alpha/LZVN) 提供了某些 HDMI 部分的补丁  
- [07151129](https://github.com/07151129) 提供了部分的代码和很多重要建议  
- [roddy20](https://github.com/roddy20) 训练和研究的编解码器补丁
- [vit9696](https://github.com/vit9696) 编写了软件并持续维护它
- [Andrey1970AppleLife](https://github.com/Andrey1970AppleLife) 和 [vandroiy2013](https://github.com/vandroiy2013) 一直在维护Codec数据库

#### 安装
最简单的安装介绍就在 [wiki](https://github.com/acidanthera/AppleALC/wiki) 上。
最新版本的 AppleALC 驱动程序在 [Releases](https://github.com/acidanthera/AppleALC/releases) 页面。

#### 贡献项目以适配更多机型
如果你想为更多机器适配不同的 Codec，你需要提供你的配置文件。阅读[wiki](https://github.com/acidanthera/AppleALC/wiki) 来获取更多信息。如果你想要贡献你的代码，可以看到在很多代码文件头部有 AppleDOC 注释供您参考。

#### 更多支持与讨论
[InsanelyMac上的帖子（英语）](http://www.insanelymac.com/forum/topic/311293-applealc-—-dynamic-applehda-patching/)  
[AppleLife上的帖子（俄语）](https://applelife.ru/threads/applealc-dinamicheskij-patching-applehda.1171672/)

#### 捐赠
编写代码和持续维护非常有趣，但同样需要大量的时间。如果你想感谢这个作者的工作，可以考虑提出 issue，提交 Bug 来帮到更多的人。
