# v0.2 validation - October 28, 2026

Release DLL SHA256: `EC5086EFF1BD4EB8430EAD4589F9690A871EAED887B8A296B99AB803A3FECFFE`

Windows 11, GOG executable SHA256:
`78C66B2EEA09A4945FE29CA0F7EE19B936E3D9E9CB32D2EC4349ABE39DD4DB71`.

## Final binary checks

- Clean x86 build using the explicit compatibility-only source list. No trainer modules.
- 22 viewport assertions passed with assertions enabled (-UNDEBUG): native, resized, letterboxed and outside-window coordinates.
- Actual drag-anchor callback regression passed: 10,000 stationary samples at (800,600) produce no accumulated pan; a 20-pixel motion remains 20 pixels.
- Launched the final DLL in a separate game copy. Enlarged main-menu labels were visible and clickable.
- Opened the enlarged Load Game list, selected the manual post-week save, and loaded it successfully. The newspaper and diplomacy result from the completed week were restored.
- Opened the city map. A right click at (800,500), beyond the former wrap thresholds, left the map in the same position.
- Closed normally with Alt+F4; the game process exited.

## Earlier build in this same validation session

The mouse fix was also tested in the organizer's small order-target map and the live working-week street view. The formerly reproducible right-click jump to the map edge stopped. Left-click territory selection worked. A populated working week completed, returned to the organizer, produced a weekly autosave and a manual save, and exited normally.

That full-week run used DLL `D3843ED133165EA4924C3F4C4A619C6E9A7606530D1E66ED91F9FF99EFCF6648`. The subsequent changes preserve listbox height during font updates and clear destroyed control records; they do not alter the game simulation or mouse-anchor callback. The final binary was rechecked for enlarged loading, restored state, map right-click stability and exit, but the entire week was not repeated after those UI-only changes.

## Scope and limits

The Windows test driver can issue right clicks but cannot hold the right button throughout a drag. Therefore a full continuous held-right-button gesture is not claimed as automated runtime coverage. The callback regression exercises its stationary/moving coordinate behavior directly. Release-and-repeat panning replaces legacy cursor wrapping at the window boundary.

F8/trainer code is excluded by the build. Pressing F8 during the checks did not open a trainer; recognition of that short synthetic keypress by the original DirectInput menu was not established.

Resizing an already-open main menu can leave stale button artwork until another menu is entered; labels remain usable. Multiplayer, exclusive fullscreen, other hardware, every save and indefinite crash-free play are not certified. No claim is made that this patch fixes the separate intermittent vehicle/weekly-transition crash.

User saves were backed up before testing; the original L slot was restored and its SHA256 matched its pre-test backup. No game executable, asset, save or private helper is distributed.

Prior v0.1 evidence is preserved in TESTING-v0.1.md.
