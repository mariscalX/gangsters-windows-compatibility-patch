# Gangsters: Organized Crime - Windows Compatibility Patch

Version 0.1 - October 25, 2026

## About

An adaptation of the community goc_compatability v0.2.0 patch:
https://github.com/xsixbulletsx/goc_compatability

Additional credits: cnc-ddraw and miniaudio. Their notices are included.
Requires your own GOG copy of Gangsters: Organized Crime.

## Fixes inherited from the community patch

- DirectDraw compatibility through cnc-ddraw.
- A guard against non-positive sprite dimensions that caused New Game crashes.
- Alternative music playback and per-user settings.

## Changes in this adaptation

- Button labels also render at native window size, without upscaling.
- Button artwork uses surface-relative coordinates instead of desktop coordinates.
- Genuine Windows API functions are resolved to avoid wrapper conflicts.
- The in-game cursor follows the actual pointer position inside the window.
- Windows button-state polling helps recover short clicks.
- A bordered-window configuration is included; fullscreen is not forced.

## Verified operation

The following checks passed on Windows 11 with the GOG version:

- Start the game and create a new game without the previous crash.
- Read and use windowed menu buttons.
- Use the aligned in-game cursor and mouse clicks.
- Save a new game to slot 001 and load it from within the game.
- Start the working week and run the populated city with pedestrians and vehicles.
- Accelerate time and complete the working week, returning to the organizer.
- Create a second manual save after the weekly transition; the weekly autosave also appeared.
- Close the game normally using the window close button and launch it again.
- Load the second save from the initial menu and verify that Week 2 was restored.

Tested executable SHA256:
78C66B2EEA09A4945FE29CA0F7EE19B936E3D9E9CB32D2EC4349ABE39DD4DB71

The test covers the included bordered-window profile, a 1024x768 in-game resolution,
and one Windows 11 PC. It does not establish multiplayer or other hardware coverage.
See TESTING.md for the reproducible test record.

No economy, recruitment, combat or diplomacy rules are changed.
The game executable remains unchanged on disk; code fixes are applied in memory.

## Installation

1. Close the game and back up its current folder and configuration.
2. Do not stack this package with another DxWrapper or DxWnd setup. Back up and
   remove conflicting wrapper DLLs/configuration before installing.
3. Copy ddraw.dll, cnc-ddraw.dll, ddraw.ini, gangsters_patch.ini and the Shaders
   folder next to gangsters.exe. Keep the included GangstersPatch_info notices.
4. Launch normally. Administrator mode and legacy compatibility modes are not required.
5. The package includes the tested windowed profile. It does not include the
   development watchdog or change global Windows preferences.

## Uninstalling

Close the game and restore the files from your backup.
If the added files did not previously exist, remove only the files/folders copied
from this package. Do not delete gangsters.exe, game data or saved games.

## Source

The source folder contains the two modified files and changes.patch against the
community project. Compilation uses that project's build.ps1 and Zig.
The original game's code, graphics, music and saved games are NOT included.
The patch DLL is not digitally signed.

