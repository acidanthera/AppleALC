ResourceGenerator for AppleALC
==============================

#### Introduction

Compiles patches provided by users to equivalent source code (i.e. `kern_resources.cpp`).

**`ResourceGenerator` is brought to you by [FireWolf](https://www.firewolf.science/) and intended to replace `ResourceConverter` to provide faster resource compilation and better error handling.**

#### Notes

- `ResourceGenerator` is written in Swift 4.1 and requires Xcode 9 to compile.
- `ResourceGenerator` supports macOS 10.9+.
- `ResourceGenerator` is not enabled now. 
- In order to use it as the default resource compiler, replace `ResourceConverter` with `ResourceGenerator`  in AppleALC's `Target Dependencies` in the `Build Phases` tab.

#### Changelog

2018.04.15
- Initial version providing resource compilation support.


