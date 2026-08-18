# Libre Nudge
<!--General badges-->
![C++](https://img.shields.io/badge/C%2B%2B-f34b7d?style=for-the-badge&logo=cplusplus)
![LICENSE](https://img.shields.io/badge/LICENSE-GPL--3.0--or--later-orange?style=for-the-badge&logo=gplv3)
![commit activity](https://img.shields.io/github/commit-activity/t/STupidLord/libre-nudge?style=for-the-badge&logo=git)
![last commit](https://img.shields.io/github/last-commit/STupidLord/libre-nudge?style=for-the-badge&logo=git)

<!--GitHub badges-->
![GitHub contributors](https://img.shields.io/github/contributors/STupidLord/libre-nudge?style=for-the-badge&logo=github)
![GitHub Issues](https://img.shields.io/github/issues/STupidLord/libre-nudge?style=for-the-badge&logo=github)
![GitHub Issues](https://img.shields.io/github/issues-closed/STupidLord/libre-nudge?style=for-the-badge&logo=github&color=8250df)
<br> <!--Line break for formatting-->
![GitHub Pull Requests](https://img.shields.io/github/issues-pr/STupidLord/libre-nudge?style=for-the-badge&logo=github)
![GitHub Closed Pull Requests](https://img.shields.io/github/issues-pr-closed/STupidLord/libre-nudge?style=for-the-badge&logo=github&color=8250df)


A open source implementation of Hearts of Iron IV's Nudge tool!

## Goals of this project
1) Cover all uses of the in-game Nudge tool
2) Implement both CLI and GUI frontends
3) Add additonal features the Nudge lacks

## Versioning
Libre Nudge consists of multiple things and thus has multiple versions. However, the project is grouped under one super-version to make it simpler on end users. The super-version is set in the root project CMakeLists file, however the GitHub Workflow uses the most recent release tag as the source for the super-version, primarily used to keep it consistent with both the release title and the installer name.

The root project and subprojects use template files to generate define macros for use in C++. The interface library `libre-nudge-config` contains the path to all of the macro headers in the build directory. It's not recommended to manually change them as rebuilding (or reconfiguring) the project will regenerate the data.

## Building
TBA, I don't want to write this up right now, I'll try and remember to do it later!

## Changelog
Later :)

## Contributing
While I most certainly would not be against help, this is currently a learning project. This section would be changed in the future in the case that I do start accepting help!

## License
This project is licensed under GPL v3 or later.

Licenses of all third party code are located in [THIRD-PARTY-LICENSES](THIRD-PARTY-LICENSES.txt).
