# Libre Nudge

![C++](https://img.shields.io/badge/C%2B%2B-f34b7d?style=for-the-badge&logo=cplusplus)
![LICENSE](https://img.shields.io/badge/LICENSE-GPL--3.0--or--later-orange?style=for-the-badge&logo=gplv3)
![GitHub commit activity](https://img.shields.io/github/commit-activity/t/STupidLord/libre-nudge?style=for-the-badge&logo=git)
![GitHub last commit](https://img.shields.io/github/last-commit/STupidLord/libre-nudge?style=for-the-badge&logo=git)

![GitHub contributors](https://img.shields.io/github/contributors/STupidLord/libre-nudge?style=for-the-badge&logo=github)
![GitHub Issues](https://img.shields.io/github/issues/STupidLord/libre-nudge?style=for-the-badge&logo=github)
![GitHub Issues](https://img.shields.io/github/issues-closed/STupidLord/libre-nudge?style=for-the-badge&logo=github&color=8250df)
![GitHub Pull Requests](https://img.shields.io/github/issues-pr/STupidLord/libre-nudge?style=for-the-badge&logo=github)
![GitHub Closed Pull Requests](https://img.shields.io/github/issues-pr-closed/STupidLord/libre-nudge?style=for-the-badge&logo=github&color=8250df)


A open source implementation of Hearts of Iron IV's Nudge tool!

## Goals of this project
1) Cover all uses of the in-game Nudge tool
2) Implement both CLI and GUI frontends
3) Add additonal features the Nudge lacks

## Versioning
It might vary well come off as confusing, but Libre Nudge has multiple different versioning numbers.

Currently there are three of these:
- CLI version ([CMakeLists.txt](cli/CMakeLists.txt) and [version.hpp](cli/include/version.hpp))
- Backend version ([CMakeLists.txt](core/CMakeLists.txt) and [version.hpp](core/include/libre-nudge/version.hpp))
- Installer version ([clang.iss](inno/clang.iss))

The installer version can be best thought of as the projects *overall* version, and the versions of the CLi and backend being for keeping track of *what* changes.

## Building
TBA, I don't want to write this up right now, I'll try and remember to do it later!

## Changelog
Later :)

## Contributing
While I most certainly would not be against help, this is currently a learning project. This section would be changed in the future in the case that I do start accepting help!

## License
This project is licensed under GPL v3 or later.

Licenses of all third party code are located in [THIRD-PARTY-LICENSES](THIRD-PARTY-LICENSES.txt).
