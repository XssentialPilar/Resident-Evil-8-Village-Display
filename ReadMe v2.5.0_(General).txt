RESIDENT EVIL VILLAGE — GENERAL GUIDE v2.5.0
RE8Display + Agility SDK 722 + DirectStorage 1.4
============================================================

1. WHAT EACH PACKAGE INSTALLS
------------------------------------------------------------

RE8Display-2.5.0.zip
  Main patch: aspect ratio, FOV, proportional interface scaling
  and the Windows cursor in menus recognized by the plugin.

RE8DirectXAgility-722-Game (Optional).zip
  Optional module: selects the Agility SDK 722 runtime before
  the game creates its DirectX 12 device.

RE8DirectStorage-1.4-Game.zip
  Optional module: uses DirectStorage 1.4 for supported PAK reads
  into CPU memory. The game retains its original decompression.

You can install RE8Display alone. Agility and DirectStorage do not
depend on each other and are not required for the display patch.

Files named Compatibility.zip are earlier standalone diagnostics.
Do not install them inside the game; they do not replace the Game.zip
modules. Use the final DirectStorage package named above: the earlier
DirectStorage Game (Optional).zip candidate does not contain the final
validated DLL.


2. PREPARATION AND REQUIREMENTS
------------------------------------------------------------

1) Close RE8 completely before installing, updating or removing modules.

2) Find the folder containing re8.exe. This is the game root directory.
   In Steam, open the game's properties and find the option to browse
   its installed files.

   Example game root:
   C:\Program Files (x86)\Steam\steamapps\common\Resident Evil Village BIOHAZARD VILLAGE

3) RE8Display and DirectStorage require an RE8-compatible x64 REFramework
   host with plugin API 1.15 or newer. Its dinput8.dll loader must be in
   the game root. Use an official build appropriate for your game:
   https://github.com/praydog/REFramework-nightly/releases

   These ZIPs do not include or replace REFramework. The presence of
   dinput8.dll alone does not establish its API version; check the
   framework log if a plugin is rejected because of its required API.

4) Back up your existing settings and mod files. If upgrading from an
   older bundled version of this patch that included a loader and
   scripts, use that version's original uninstaller and keep its backups.
   If that process removes the host, reinstall a compatible REFramework.

5) Disable REFramework's separate Graphics ultrawide/FOV correction and
   other patches that control the same camera or interface fields.
   Do not run two corrections for the same aspect ratio/FOV fields
   simultaneously.

6) Agility and DirectStorage use specific Microsoft preview packages.
   Their runtimes are not redistributed in our ZIPs. Keep the original
   NuGet packages, read their licenses and accept their terms before
   using -AcceptPreviewLicense. These installers are intended for the
   private testing permitted under those terms.

   Agility:
   microsoft.direct3d.d3d12.1.722.0-preview.nupkg

   DirectStorage:
   microsoft.direct3d.directstorage.1.4.0-preview1-2603.504.nupkg

   Do not extract or rename the .nupkg files. The installer verifies
   the exact package, hashes and Microsoft runtime signatures.


3. INSTALL RE8DISPLAY v2.5.0 — MAIN PATCH
------------------------------------------------------------

1) Extract RE8Display-2.5.0.zip.
2) Copy the extracted reframework folder into the root containing re8.exe.
   Merge the folders. Do not create a reframework folder inside another.
3) Confirm that the files are at these exact locations:

   GAME ROOT\reframework\plugins\RE8Display.dll
   GAME ROOT\reframework\plugins\RE8Display.LICENSE.txt
   GAME ROOT\reframework\data\RE8Display.ini

   When updating this plugin, back up your INI first if you want to
   retain personal settings. Do not overwrite files from other mods.

4) Start the game normally. Press ; to open Aspect Ratio Config Menu.
   Change menu key lets you assign another shortcut. Reset menu key to ;
   restores the semicolon character for your keyboard layout.
   The REFramework host has its own menu shortcut; it is separate
   from the patch's menu.

5) Automatic FOV and proportional interface scaling are enabled by default.
   Override FOV is optional and uses degrees referenced to the 16:9 base.
   Manual FOV Multiplier and Keep Black Bars in 16:10 have been removed.

The map retains the game's original controls. Additional mouse dragging
for the map is not implemented in this release.


4. INSTALL AGILITY SDK 722 — OPTIONAL
------------------------------------------------------------

1) Extract RE8DirectXAgility-722-Game (Optional).zip into its own folder
   outside the game. Do not merge it with the DirectStorage ZIP:
   both contain install.ps1, uninstall.ps1 and common.ps1 scripts.

2) Open PowerShell in the extracted folder containing install.ps1 and
   package.json. Keep all its files and the payload folder together.

3) Adjust both example paths and run these lines in that window:

   $Game = 'C:\Games\Resident Evil Village'
   $SDK = 'C:\RE8-Mods\SDK\microsoft.direct3d.d3d12.1.722.0-preview.nupkg'
   powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\install.ps1 -GamePath $Game -PackagePath $SDK -AcceptPreviewLicense

   $Game must point to YOUR folder containing re8.exe.
   $SDK must point to the complete original .nupkg file.
   This command does not install the SDK globally or permanently change
   the Windows execution policy.

4) The installer places its owned files at:

   GAME ROOT\d3d12.dll
   GAME ROOT\RE8Agility\D3D12\D3D12Core.dll
   GAME ROOT\RE8Agility\LICENSE-preview.txt
   GAME ROOT\.RE8Technologies\Agility\install-manifest.json

   Installation is refused if d3d12.dll or another target already exists.
   Do not delete another mod's proxy to force installation. If the
   existing files belong to this module, use its uninstaller before
   updating.

5) Start RE8 normally and inspect:

   GAME ROOT\RE8Agility\bridge.log

   Look for ACTIVATED and a successful device creation showing:
   FIRST_DEVICE HRESULT=0x00000000 active=1 device_sdk=722

   DECLINED means that native forwarding was retained; it does not
   confirm activation. Copying the proxy without the private runtime
   does not complete installation.

Do not replace Windows DLLs or files in System32 or SysWOW64. Do not copy
the Core directly into the game root or copy every NuGet file into the
game. This module selects the runtime; it does not automatically enable
every new rendering feature offered by that SDK.


5. INSTALL DIRECTSTORAGE 1.4 — OPTIONAL
------------------------------------------------------------

1) Extract RE8DirectStorage-1.4-Game.zip into another dedicated folder,
   outside the game and separate from Agility. Keep all its files together.

2) Confirm that REFramework with plugin API 1.15 or newer is installed.
   DirectStorage requires that host; installing Agility does not replace it.

3) Open PowerShell in the extracted folder containing install.ps1 and
   package.json. Adjust both paths and run:

   $Game = 'C:\Games\Resident Evil Village'
   $SDK = 'C:\RE8-Mods\SDK\microsoft.direct3d.directstorage.1.4.0-preview1-2603.504.nupkg'
   powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\install.ps1 -GamePath $Game -PackagePath $SDK -AcceptPreviewLicense

4) The installer prepares:

   GAME ROOT\reframework\plugins\RE8DirectStorage.dll
   GAME ROOT\RE8DirectStorage\dstorage.dll
   GAME ROOT\RE8DirectStorage\dstoragecore.dll
   GAME ROOT\RE8DirectStorage\LICENSE-preview.txt
   GAME ROOT\RE8DirectStorage\NOTICES-preview.txt
   GAME ROOT\.RE8Technologies\DirectStorage\install-manifest.json

   Do not copy only the DLL from payload or put the runtimes in System32
   or the game root. Use the installer and its private runtime paths.

5) Start RE8, load a save and explore an area. Then inspect the
   [RE8DirectStorage] lines in:

   GAME ROOT\re2_framework_log.txt

   Look for FIRST REAL PAK READ and counters showing active=1,
   pak_reads > 0 and pak_bytes > 0. After enough supported reads,
   verification_reads should reach 64. The verification_mismatch,
   verification_errors, native_compat_errors, timeout_disable, runtime,
   allocation and copy counters must remain zero.

   A plugin-loaded message alone does not prove DirectStorage reads.
   native_fallbacks can increase: requests outside the bridge's scope
   retain the Windows reader and are not necessarily errors.

The final DLL is the same binary that completed 18,011 PAK reads and
4,543,855,912 bytes in the recorded test. The first 64 reads matched
independent native reads completely, with no mismatches, errors or
timeouts. The user completed gameplay without loading errors or hangs.

The implemented path reads PAK files into CPU memory. It does not provide
GPU decompression or establish an FPS improvement. Validation covers the
tested hardware, executable and gameplay session; it is not a universal
guarantee.


6. VERIFICATION ORDER AND INSTALLATION ISSUES
------------------------------------------------------------

Suggested order: REFramework, RE8Display, then each optional module
separately. Launch the game and verify each installation before adding
the next. Close RE8 between installations. Neither optional installer
will close the game for you.

If the installer reports an existing installation or recovery transaction,
use that module's uninstall.ps1 before trying again. Do not delete the
manifest to bypass ownership checks. An interrupted installation may
require the same recovery process.

If you see Wrong original Microsoft preview package, confirm that you
selected the exact .nupkg listed in this guide. Do not disable signature
or hash verification.

If access is denied, check write permissions for the game folder.
If that location requires elevation, run the installer from PowerShell
with the necessary permissions.

If you encounter a black screen, flickering or loading failures, keep
the logs and remove the most recently added module with the game closed.
Test again to isolate the cause. Activation of the optional modules
does not itself verify visual correctness for every resolution, shadow,
menu or combination of other mods.


7. REMOVE OR UPDATE EACH COMPONENT
------------------------------------------------------------

AGILITY OR DIRECTSTORAGE:
  Close RE8. Open PowerShell in the extracted folder of the module you
  want to remove. Adjust $Game and run:

  $Game = 'C:\Games\Resident Evil Village'
  powershell.exe -NoProfile -ExecutionPolicy Bypass -File .\uninstall.ps1 -GamePath $Game

  Run this command from the Agility folder to remove Agility, or from
  the DirectStorage folder to remove DirectStorage.
  The uninstaller checks hashes and its ownership manifest before
  removing owned files. If it detects changed files, removal is refused:
  investigate the changes and keep a copy before resolving them.
  Logs and files belonging to other mods are preserved.

  To update, remove the previous installation of that module first,
  then run the new install.ps1. Keep the extracted folders, original
  packages and manifests for recovery.

RE8DISPLAY INSTALLED BY COPYING THE ZIP:
  With RE8 closed, remove only:
  reframework\plugins\RE8Display.dll
  reframework\plugins\RE8Display.LICENSE.txt

  You may keep reframework\data\RE8Display.ini to retain settings,
  or remove it if you also want to delete the private configuration.
  Do not delete the entire reframework folder or dinput8.dll.

  If installed through the development tools with a .RE8Display
  manifest, use the corresponding uninstaller to restore that
  transaction's backups.


8. VERIFY THIS DELIVERY'S ZIP FILES
------------------------------------------------------------

In the folder containing the ZIP files, use PowerShell to view a hash:

  Get-FileHash -LiteralPath '.\RE8DirectStorage-1.4-Game.zip' -Algorithm SHA256

Hashes of the packages checked when this guide was prepared:

RE8Display-2.5.0.zip
3DA8E5DB233F7654362655B848FA6424A5ED442BE664450CB99893C79FF994FF

RE8DirectXAgility-722-Game (Optional).zip
782C2C5C37322F4F26C9639FD2F210171AC0726C68EB471C11266CC2DBFE0CCD

RE8DirectStorage-1.4-Game.zip
1F94C285650725BA013277A5C8A755D8F5994C79CFE1F7217C8FAB3096E553CC

Validated DirectStorage DLL, included without recompilation:
1D8A53EC6C9434AE08FD1E12E5CC55EE327F844D4B29BE42BDAA6607863C8094

This README is delivered alongside the ZIP files. It does not change
their contents or hashes. Our public ZIP files do not include game
assets, save files or Microsoft preview runtime binaries.
