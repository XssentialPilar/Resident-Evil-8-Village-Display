# Building the saved 2.7.2 source

The source ZIP preserves the code and configuration associated with the accepted DLL, together with the main plugin's vendored REFramework API and MinHook sources. It excludes build trees, personal settings, game files, logs, recovery copies and private Microsoft SDK packages.

On Windows x64, install Visual Studio with Desktop development with C++ and a Windows SDK, plus CMake. From the extracted source root, run:

```powershell
powershell -NoProfile -ExecutionPolicy Bypass -File tools/RE8Display.Build.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tools/RE8Display.Test.ps1
powershell -NoProfile -ExecutionPolicy Bypass -File tools/RE8Display.Package.ps1
```

The build helper uses the locally available Visual Studio toolchain. The validated binary was built with Visual Studio 18 / Windows SDK 10.0.26100.0. A different toolchain can produce different binary bytes.

The package command refuses to overwrite an existing release ZIP. The source snapshot's native inputs are checked against `SOURCE-MANIFEST.json`. The shipped DLL's SHA-256 is `16E9B902E5475044B917A07D3B43C1039BF1E9087F6B3134BBAEFD1D806652A4`.

Optional technology-module source is retained, but rebuilding those independent modules also requires their Microsoft packages, kept separately in the development workspace. Read `docs/release-2.7.2.md` for the accepted session's scope and remaining limitations.
