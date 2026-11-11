# Resident Evil Village — RE8Display v2.7.2

**Cinematics without distortion. Transitions that fit. The original cursor where it belongs.**

**RE8Display** is an independent [REFramework](https://github.com/praydog/REFramework) display plugin for **Resident Evil Village** on Windows. It improves aspect-ratio presentation, field of view, proportional interface scaling, and native cursor behavior, with its own settings menu and configuration.

The **v2.7.2** update focuses on **pre-recorded video framing, full-screen transitions, and interaction-prompt proportions**, while retaining the cursor, camera, and interface improvements established in earlier releases.

**[Downloads and releases](https://github.com/XssentialPilar/Resident-Evil-8-Village-Aspect-Ratio-Patch-/releases)** · **[Installation notes](START-HERE.md)** · **[Current validation and scope](docs/RE8Display-2.7.2-media-r2.md)**

## In-Game Overview

<img width="1448" height="1086" alt="RE8Display-2 7 2-overview" src="https://github.com/user-attachments/assets/a4dc4c66-4b05-491e-ae06-3a5a80ddc139" />


*Promotional overview composed from Resident Evil Village imagery. The artwork illustrates the features; it is not a substitute for the runtime tests described below.*

## What's New in v2.7.2

### Proportional Cinematics and Pre-Recorded Videos

Videos can now follow the display without being stretched horizontally or vertically. The corrected in-game video handling was observed with the **RE Engine logo and opening movie** at **1920 × 1200 (16:10)**.

Choose the presentation you prefer:

- **Fill the screen (default):** Preserves proportions while cropping the outer edges when needed.
- **Keep complete video image (with bars):** Preserves every part of the picture, with bars when the source and display have different shapes.

The video-framing setting is enabled by default. Playback timing and audio are left to the game.

### Adaptive Full-Screen Transitions

Recognized full-screen transition backgrounds now adapt automatically to the output, reducing incomplete coverage when the display shape changes. This adjustment operates independently of the video-framing option; there is no additional transition switch to configure.

### Better-Proportioned Interaction Prompts

The patch now corrects a recognized vertical-scaling mismatch affecting certain on-screen interaction indicators. In the tested RE8 session, the prompt returned to proportional sizing **without shifting its position or overriding unrelated native animation changes**.

### Less Unnecessary Work

Small internal refinements avoid redundant interface updates and calls. They reduce work performed by the patch itself; **no specific FPS, VRAM, or loading-time improvement is promised**.

## Established Features, Still Included

- **Original game cursor and effects:** Enabled by default, with improved synchronization to the Windows mouse. A Windows cursor-only option remains available.
- **Aspect-ratio and camera handling:** Uses active output dimensions and maintains the native camera's automatic FOV behavior. A manual FOV override is available separately.
- **Proportional interface scaling:** Uniform adjustments help menus, HUD elements, and inventory screens retain their intended proportions.
- **Targeted menu interaction:** Cursor handling covers supported native interfaces such as inventory, the Duke's shop, map, and typewriter save menus.
- **Independent configuration:** A scrollable settings menu, rebindable shortcut, background saving, and **Restore All Defaults** are retained from previous versions.

**Map controls remain native to the game.** RE8Display does not introduce a separate map-dragging system.

## Requirements

| Component | Requirement |
| --- | --- |
| Game | Resident Evil Village for Windows |
| Architecture | Windows x64 |
| Framework | REFramework with plugin API **1.15 or newer** — **included in the main v2.7.2 ZIP** |
| Main package | `RE8Display-2.7.2.zip` |

**The main download includes the compatible REFramework loader (`dinput8.dll`) required by RE8Display.** You do not need a separate REFramework download for this bundle. If you later install a different host version, verify plugin API compatibility. Official framework updates are available from [REFramework](https://github.com/praydog/REFramework) and its [nightly releases](https://github.com/praydog/REFramework-nightly/releases).

## Installation and Upgrading

1. **Close Resident Evil Village.** If upgrading, back up the existing `dinput8.dll`, `reframework/plugins/RE8Display.dll`, and `reframework/data/RE8Display.ini` where present.
2. Download the main **`RE8Display-2.7.2.zip`**, which includes both the compatible REFramework host and the display patch.
3. Extract the contents into the game directory containing **`re8.exe`**. Keep the archive's directory structure and allow the `reframework` folders to merge. Back up any existing `dinput8.dll` before replacing it.
4. To retain your preferences, preserve the existing `RE8Display.ini` instead of replacing it with the packaged defaults.
5. Launch Village and press **`;`** (or your configured shortcut) to open the independent **RE8Display Settings** menu.

The expected installation layout is:

```text
Resident Evil Village/
├── re8.exe
├── dinput8.dll
└── reframework/
    ├── plugins/
    │   ├── RE8Display.dll
    │   └── RE8Display.LICENSE.txt
    └── data/
        └── RE8Display.ini
```

The bundle does not replace game saves or Windows DirectX system files. The optional Agility and DirectStorage modules are separate downloads and are **not required**.

**Important:** Use only one installed RE8Display revision. Disable REFramework's separate ultrawide correction and any other mod making competing camera or interface adjustments if you encounter conflicting results.

## Settings at a Glance

| Setting | Purpose |
| --- | --- |
| **Correct pre-recorded video framing** | Enabled by default; proportionally adapts supported movies to the output. |
| **Keep complete video image (with bars)** | Optional; shows the entire video frame instead of cropping to fill. |
| **Use original game cursor and effects** | Retains the original cursor appearance; can be disabled for the Windows pointer alone. |
| **Cursor control in game menus** | Manages pointer behavior in supported native menus. |
| **Enable display correction** | Controls display and camera adaptations. |
| **Override FOV** | Optional manual FOV control; automatic handling is the default. |
| **Use vertical FOV** | Selects the vertical FOV axis when applicable. |
| **Proportional game interface** | Enables uniform screen-interface adjustment. |
| **Change menu key** | Rebinds the independent menu shortcut, initially `;` on the active keyboard layout. |
| **Restore All Defaults** | Resets the patch's settings and shortcut. |
| **Diagnostics** | Enables additional logs when investigating problems. |

Full-screen transitions adapt automatically when their recognized native backgrounds are available; they do not have a separate option. REFramework's own menu and settings remain independent.

## Validation and Compatibility

The final **2.7.2 media revision** was tested in Resident Evil Village and accepted on **November 11, 2026**. The recorded session confirms active movie and transition bindings, proportional video rendering at **1920 × 1200**, and the corrected interaction-prompt scale while retaining its position.

Automated checks passed:

- **10 standalone C++ tests** for display policies, plugin compatibility, video bindings, and prompt geometry.
- **27 installation checks** for installation and recovery behavior.
- **21 package checks** for distributable layout and integrity.

Testing at 16:10 and automated geometry cases **does not establish universal support** for every video, display ratio, game build, or combination of mods. Unrecognized movie structures are left unchanged rather than forcefully adjusted. Some archived REFramework diagnostics are unrelated to the successful video and prompt corrections and are not claimed fixed here.

For the current record, see the [final revision notes](docs/RE8Display-2.7.2-media-r2.md) and [validation data](docs/validation-2.7.2-media-r2.json). Older 2.7.2 candidate reports do not supersede the final tested revision.

## Troubleshooting and Removal

| Symptom | What to check |
| --- | --- |
| Plugin does not load | Confirm `dinput8.dll` is next to `re8.exe`, the plugin is under `reframework/plugins/`, and the host supports plugin API 1.15+. |
| Video still looks stretched | Enable **Correct pre-recorded video framing**; try switching the complete-image option and report the affected movie. |
| Bars appear around a video | Disable **Keep complete video image (with bars)** if you prefer proportional fill with cropped edges. |
| Unexpected aspect ratio or FOV | Turn off competing camera and ultrawide corrections; try automatic FOV handling. |
| Cursor or shortcut does not behave as expected | Check the two cursor options, custom menu binding, and active keyboard layout. |
| Personal settings changed after upgrading | Restore your backed-up `RE8Display.ini` or use **Restore All Defaults**. |

To remove only RE8Display, close the game and remove `RE8Display.dll` and its corresponding license notice from `reframework/plugins/`. Remove `reframework/data/RE8Display.ini` only if you want to discard your preferences. **Keep `dinput8.dll` if other mods still require REFramework.** To restore a previous host, use your backed-up loader. Optional modules have their own installation and removal procedures.

## Optional Technology Modules

These are **separate downloads**, unchanged by this display and media revision. The main RE8Display patch works without them.

### DirectX 12 Agility SDK 722

**Optional package:** `RE8Agility-722-Game-2.7.2 (Optional).zip`

This bridge allows RE8 to select a private Agility SDK 722 runtime at startup and report the graphics capabilities actually available on the game device. Its activation was verified during earlier combined-module testing. It does not replace system DirectX files or automatically introduce newer rendering features.

### DirectStorage 1.4

**Optional package:** `RE8DirectStorage-1.4-Game-2.7.2 (Optional).zip`

This bridge routes eligible read-only game-archive requests through DirectStorage into CPU memory, retains the game's original decompression path, and provides native fallback and verification. Earlier combined-module testing recorded **64 successful native comparisons** without verification failures. The module was not changed by this release's video corrections.

**Scope:** Neither module guarantees FPS, VRAM, or loading-time improvements. They do not add newer DXR features or GPU asset decompression. Each optional module requires its matching original Microsoft preview package, which is not redistributed in the optional ZIPs.

## Building from Source

Windows x64, CMake 3.20 or newer, and a Visual Studio C++ toolchain supporting C++23 are required. The source archive includes the plugin's required SDK and MinHook source dependencies.

```powershell
powershell.exe -NoProfile -ExecutionPolicy Bypass -File tools/RE8Display.Build.ps1
powershell.exe -NoProfile -ExecutionPolicy Bypass -File tools/RE8Display.Test.ps1
```

The source build and installation bundle serve different purposes: the distributed v2.7.2 game ZIP additionally contains the compatible REFramework host. The build receipt and checksums identify the final tested plugin binary.

## Credits and Feedback

**Maintained by [XssentialPilar](https://github.com/XssentialPilar).**

- [REFramework](https://github.com/praydog/REFramework) and its public SDK — Praydog and contributors.
- [MinHook](https://github.com/TsudaKageyu/minhook) — API interception dependency.
- **Resident Evil Village / RE Engine** — Capcom.

This is an independent community project, not affiliated with or endorsed by Capcom. Required license notices accompany the distributable plugin; game artwork is not bundled in the runtime ZIP.

Thank you for supporting RE8Display. If you find a problem, please [open an issue](https://github.com/XssentialPilar/Resident-Evil-8-Village-Aspect-Ratio-Patch-/issues) and include the affected screen or video, resolution, aspect ratio, display mode, REFramework version, installed optional modules, and steps to reproduce it. Screenshots or short recordings help make future updates more reliable.
