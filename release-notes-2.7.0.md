# RE8Display v2.7.0

**The original game cursor returns, with improved tracking, clearer settings, and reduced patch overhead.**

RE8Display 2.7.0 refines the way you interact with Resident Evil Village while continuing to provide aspect ratio, field of view, and proportional interface adjustments. This release focuses on cursor alignment, more efficient operation, and easier configuration.

## What's New

### Original Game Cursor Restored

The game's original cursor and visual effects are enabled by default. Its displayed position follows the Windows mouse position directly, addressing the offset observed in earlier builds while preserving the cursor's proportions and native appearance. You can switch to the Windows cursor alone at any time through the settings menu.

### Reduced Patch Overhead

Repeated camera and interface adjustments have been reduced, and configuration changes are saved in the background. These improvements reduce unnecessary work during gameplay and when adjusting settings, helping limit interruptions caused by the patch itself.

### Refined Configuration Menu

Settings are arranged into clearer sections, with a full-width FOV slider and a dedicated cursor mode option. A **Restore All Defaults** button resets every patch setting, including automatic FOV handling and the default menu shortcut, making it easier to return to the original configuration.

### Improved Display Detection

Display adjustments now use the actual rendered output dimensions, with monitor information reported separately. The patch also responds to resolution, window, display, and DPI changes, improving how it updates its settings when your display configuration changes.

## In-Game Screenshots

<!-- Add two or three in-game screenshots here using GitHub's image upload feature. -->

## Installation

1. Close Resident Evil Village and back up your existing patch files and settings if you are upgrading.
2. Ensure a compatible version of **REFramework with plugin API 1.15 or newer** is installed.
3. Extract **`RE8Display-2.7.0.zip`** into the game directory containing `re8.exe`, allowing the `reframework` folders to merge.
4. Launch the game and press **`;`** to open the settings menu.

The main ZIP contains only the display plugin, its default configuration, and the required license notices. Please refer to **`ReadMe v2.7.0_(General).txt`** for further installation and rollback instructions.

## Testing and Feedback

The corrected cursor and the combined optional modules passed user gameplay testing, with no reported loading failures, crashes, or new stuttering during the tested session. Automated checks also covered installation, recovery, and package integrity.

Thank you for your feedback. Please include your resolution, display mode, REFramework version, and reproduction steps when reporting an issue.

## Optional Modules

The following modules are provided as **separate downloads**. Neither is required to use the main display patch. Please follow each module's included installation guide rather than copying its payload files directly into the game folder.

### DirectX 12 Agility SDK 722

**Download:** `RE8Agility-722-Game-2.7.0.zip`

This startup bridge allows RE8 to select a private Agility SDK 722 runtime before creating its graphics device. It also records the actual device SDK and supported graphics capabilities, providing confirmation that the runtime is active. SDK 722 activation was verified inside the game during this release's testing.

### DirectStorage 1.4

**Download:** `RE8DirectStorage-1.4-Game-2.7.0.zip`

This bridge routes eligible, read-only PAK file requests through DirectStorage into CPU memory while retaining the game's original decompression process. It includes native fallback and verification checks. Real game reads were confirmed during testing, with 64 successful native comparisons and no recorded verification errors or timeouts.

> **Technical scope:** These bridges do not port RE8 to new DXR features such as clustered geometry, PTLAS, SER, or OMM. Zstd/GACL asset conversion and GPU decompression are not implemented. They do not replace Windows system DirectX files or guarantee FPS, VRAM, or loading-time improvements. Installation requires the original matching Microsoft preview packages, which are not redistributed in these ZIPs.
