AppleALC
========

[![Build Status](https://github.com/acidanthera/AppleALC/workflows/CI/badge.svg?branch=master)](https://github.com/acidanthera/AppleALC/actions) [![Scan Status](https://scan.coverity.com/projects/16166/badge.svg?flat=1)](https://scan.coverity.com/projects/16166)

一个开源的内核扩展,为非官方的声卡提供支持的Codec,无需修改任何系统文件,来启用原生macOS高清音频.

#### 特性
- 在系统安装阶段即可使用数字或模拟音频.
- 支持恢复模式与安装模式.
- 自动加载声卡Codec.
- 启用Apple官方不支持的编解码器 (无论是内置还是外置的声卡设备).
- 任意的kext修改补丁.
- 自定义设备Layout ID号与支持平台.
- 工作在 SIP 模式 / El Capitan 系统.
- 当前版本兼容: 10.8-11.

#### 向以下代码贡献人员致谢
- [Apple](https://www.apple.com) 设计的 macOS 操作系统 
- [Onyx The Black Cat](https://github.com/gdbinit/onyx-the-black-cat) by [fG!](https://reverse.put.as) 提供了数据库和内核补丁
- [capstone](https://github.com/aquynh/capstone) by [Nguyen Anh Quynh](https://github.com/aquynh) 提供了部分反汇编程序模块 
- [toleda](https://github.com/toleda), [Mirone](https://github.com/Mirone) 以及某些其他的layout和音频补丁
- [Pike R. Alpha](https://github.com/Piker-Alpha) for [lzvn](https://github.com/Piker-Alpha/LZVN) 提供了某些HDMI部分的补丁  
- [07151129](https://github.com/07151129) 提供了部分的代码和很多重要建议  
- [vit9696](https://github.com/vit9696) 编写了软件并持续维护它
- [vandroiy2013](https://github.com/vandroiy2013) 一直在维护Codec数据库

#### 如何安装?
现在已经在WIKI上提供了最简单的安装介绍 [点我](https://github.com/vit9696/AppleALC/wiki).  
想下载最新版本的AppleALC驱动程序? 点它跳转到下载页→[Releases](https://github.com/vit9696/AppleALC/releases).

#### 我如何为这个AppleALC提供更多的Codec数据来适配更多的机器?
如果你想为更多不同的机器适配不同的Codec,你需要提供你的配置文件. 请阅读[基本指南](https://github.com/vit9696/AppleALC/wiki)获取更多信息. 
如果你是一个程序开发者想要贡献你的代码,可以看到在很多代码文件头部就有很重要的AppleDOC注释供你参考.

#### 更多的支持与讨论(需要绿色上网环境以提升更好的体验)
[InsanelyMac上的帖子](http://www.insanelymac.com/forum/topic/311293-applealc-—-dynamic-applehda-patching/) in English  
[AppleLife上的帖子](https://applelife.ru/threads/applealc-dinamicheskij-patching-applehda.1171672/) in Russian

#### 关于捐赠
编写代码和持续维护非常有趣,但同样需要大量的时间. 如果你想感谢这个作者的工作,可以考虑提出issue, BUG报告,或为更多的人提供帮助.
