# RE8Display 2.7.2 Final — Installation and revision notes

This revision addresses the video bindings that were unavailable in the previous 2.7.2 build. It also corrects a recognized aspect-ratio compensation on Village's interaction prompts. The accepted cursor policy, camera policy, menu layout and mandatory transition correction are retained.

## What changed

- Video methods are resolved through their actual inherited engine types. The captured RE Engine logo and opening movie use this path. Decoder dimensions and the authored image size determine proportional framing, including supported codec padding.
- Fill mode uses uniform scaling and centered cropping. Complete-image mode uses uniform scaling with bars. Neither mode stretches the two axes independently.
- Interaction prompt correction targets `GUIInteractIcon/main` only when its vertical scale matches the observed aspect compensation. Its position and unrelated animation values are preserved.
- Small engine calls use stack storage. Unchanged GUI properties avoid redundant writes and readback calls. No measured frame-rate or latency improvement is claimed.

## Install

1. Close Resident Evil Village normally.
2. Keep the compatible REFramework already installed for Village. This patch ZIP does not include `dinput8.dll` and does not replace the optional Agility or DirectStorage modules.
3. Back up `reframework/plugins/RE8Display.dll` and `reframework/data/RE8Display.ini`.
4. Extract `RE8Display-2.7.2-Final.zip` directly beside `re8.exe`, retaining its folder structure.
5. For an upgrade, preserve your existing INI instead of replacing it with the supplied defaults if you want to retain your preferences.
6. Open the patch menu using your configured shortcut. Enable **Correct pre-recorded video framing**. Leave **Keep complete video image (with bars)** off for proportional fill, or enable it to retain the entire image.

Transitions remain automatically adapted. Use only one installed RE8Display plugin; remove duplicate copies of this plugin rather than running multiple revisions together.

## Validation and scope

The build passed 10 standalone C++ tests, including inheritance lookup, proportional prompt geometry and plugin loading, plus 27 installer checks and 21 runtime package checks.

The user tested this exact DLL and approved it as final on November 11, 2026. Its SHA-256 is `71FC52837ED69C864AEEF32460A9EBF482BC6BDE33091E14D44C97053D8B26D0`. The recorded session confirms ready movie and transition bindings, proportional movie rendering at 1920 × 1200, and the interaction panel correction from vertical scale 0.9 to 1.0 with its position retained. This approval covers the user's tested scenarios; it does not establish exhaustive coverage of every movie, resolution or game build. Unsupported or ambiguous movie hierarchies are left unchanged instead of applying an unverified global override.

The archived host log also contains REFramework diagnostics unrelated to these successful media/prompt activation messages. This release is not described as an entirely error-free host session.

## Restore

With the game closed, restore the backed-up plugin and your INI. Do not remove or overwrite unrelated REFramework plugins or optional modules.

## Build from source

Use Windows x64 with CMake 3.20 or newer and a Visual Studio C++ toolchain supporting C++23. The source archive includes the plugin's SDK and MinHook source dependencies. The build script defaults to Visual Studio 2026; select your installed generator if different. From its extracted root, run:

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File tools/RE8Display.Build.ps1
powershell.exe -NoProfile -ExecutionPolicy Bypass -File tools/RE8Display.Test.ps1
```

The authoritative receipt for this revision is `BUILD-RECEIPT.json` in the source archive. Historical 2.7.2 documents refer to earlier accepted artifacts and do not certify this revision's runtime behavior.

The optional Agility and DirectStorage packages are distributed separately and are unchanged by this media revision. They are not required for this patch's video framing or interaction prompt correction.
