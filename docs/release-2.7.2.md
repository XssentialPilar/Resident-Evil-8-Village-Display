# RE8Display 2.7.2 — Video framing and transitions

This release adds full-screen coverage for identified transition backgrounds and video-framing controls. Existing RE8 camera, HUD, cursor synchronization and optional modules retain their 2.7.0 implementation. The video-framing implementation requires the engine bindings described under Validation status.

## Installation

Close Resident Evil Village before replacing files. Extract the ZIP beside `re8.exe`, keeping the `reframework` directory structure. An existing compatible REFramework installation is required. The ZIP does not replace `dinput8.dll`, the game's DirectX runtime, optional modules or REFramework settings. Back up your existing `reframework/data/RE8Display.ini` to preserve personal settings when extracting the default configuration.

Open the existing settings window with **;**. The new **Pre-recorded videos** section provides:

- **Correct pre-recorded video framing** — enabled by default. Uniformly fills the output, with centered cropping when the source and display proportions differ.
- **Keep complete video image (with bars)** — preserves the entire frame using uniform scaling and unused borders where needed.

Identified full-screen transitions remain adapted regardless of the cinematic setting. There is no transition toggle. Playback timing, audio, subtitles and loading-indicator visibility are not modified.

## Validation status

The user accepted the installed 2.7.2 build on November 10, 2026 and requested final packaging without further changes. The final archive preserves that exact tested DLL. Eight native/ABI tests, 27 installer checks and 21 package checks passed.

The session log confirms plugin 2.7.2, active transition bindings, full-screen transition geometry at 1920 x 1200, and the existing absolute cursor synchronization. **Movie bindings remained unavailable in this session:** the new video-framing controls cannot be claimed to have applied to prerecorded movies. The implementation is saved for follow-up; it leaves unavailable movie bindings untouched. Mathematical framing tests do not establish runtime video coverage or compatibility with every display.

REFramework also recorded `Present failed: 87a0001` during the session. That message appeared in the earlier 2.7.0 baseline too; its cause is unresolved. The user reported acceptable behavior and chose to handle remaining issues separately. This release does not claim to resolve that message.

The optional Agility and DirectStorage modules are independent and are not changed or included in this ZIP.
