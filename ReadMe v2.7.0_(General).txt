RE8Display 2.7.0 — RELEASE CANDIDATE INSTALLATION GUIDE
Windows x64 / Resident Evil Village / REFramework public API 1.15+

1. DISPLAY PATCH
Close RE8 normally before replacing files. Keep the existing 2.5.0 ZIP.
Extract the contents of the RE8Display-2.7.0 candidate ZIP directly into the
game directory beside re8.exe. The reframework directory must merge with
the existing one. A compatible REFramework dinput8.dll is included in this distribution. Back
up any existing loader before replacing it; no separate host download is needed.
Do not place the patch inside another nested game directory.

Press ; to open Settings. Use original game cursor and effects is enabled
by default. Disable that option to use the Windows cursor alone. The patch
does not duplicate clicks. Restore all defaults resets every patch option,
including automatic FOV, cursor settings and the ; shortcut.
Settings save after 250 ms without changes. Wait briefly before exiting.

The Windows pointer graphic is hidden while the game cursor is drawing.
The Windows position remains available to the game. If the original cursor
is unavailable, a Windows pointer is retained as a fallback.

2. OPTIONAL AGILITY AND DIRECTSTORAGE MODULES
Each optional module has its own ZIP and installer. Extract each ZIP to
its own directory outside the game folder. Do not copy its payload folder
blindly into the game. Read that module's README before using install.ps1.
The installer needs the original matching Microsoft .nupkg package and
the game directory. It validates inputs and creates its own rollback manifest.
Run only with RE8 closed; do not replace files in a running process.
Use the included uninstall.ps1 to remove that module and restore its backup.
Do not mix old and new copies of a module or install another d3d12.dll proxy
over the Agility bridge. Microsoft preview runtimes are not redistributed.

Agility changes runtime selection before initial device creation. The new
diagnostics report the actual device SDK and hardware capabilities. The
patch does not convert RE8's shaders or ray-tracing structures to new DXR APIs.

DirectStorage routes eligible read-only PAK requests to CPU memory with
native fallback and startup comparisons. Original decompression remains.
This does not enable Zstd, GACL or GPU decompression for existing RE8 assets.
No guaranteed FPS, memory, loading-time or universal compatibility claim is made.

3. TESTING / ROLLBACK
These exact binaries passed a fresh user gameplay test with both optional
modules. Further builds and display configurations need their own checks. Compare the
same inventory, Duke shop, save list and map with both cursor modes. Test
hover alignment, clicks, dragging, focus changes, window changes and aiming.
Record the display output and any visible disagreement between pointer
position and the game highlight. An older module's test is not proof for a
new DLL. Keep a copy of current files before manual replacement.
To revert the display patch, close RE8 and restore the preserved 2.5.0
display ZIP and your prior private INI. Shared host settings and saves are
not modified by this patch. Optional modules are rolled back separately.

The prior water/lighting defect has not been certified fixed. Full engine
integration of new DXR/Zstd/GACL features is not part of this candidate.
