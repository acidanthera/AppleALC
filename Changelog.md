AppleALC Changelog
==================

#### v1.0.2
- Added ALC269 10.9.x/10.10.x patches (by Andrey1970)
- Added ALC270 codec support (by m-dudarev)

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