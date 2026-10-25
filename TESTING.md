# Test record - October 25, 2026

Environment: Windows 11, GOG release, original executable unchanged.
Display: bordered window, 1024x768 game resolution, packaged configuration.
Patch DLL SHA256: 4C276E2C297A22D21DE00A40191ABEB6637143B993D8F5005F7030930D4CEE4F

| Check | Result | Evidence observed |
| --- | --- | --- |
| New game | PASS | Character screen and organizer loaded without the previous crash. |
| Windowed labels and controls | PASS | New Game, Start Game and Load Game used through visible buttons. |
| Mouse alignment/clicks | PASS | Dialogs, map and save/load controls operated at their visible positions; user also confirmed. |
| Initial manual save | PASS | Slot 001 named A; file A.01 created, 1,180,798 bytes. |
| In-session reload | PASS | Selected A, confirmed reload; game displayed Game loaded successfully. |
| Working week | PASS | Street view loaded; pedestrians and vehicles moved; clock advanced. |
| Accelerated time/weekly transition | PASS | Week 1 completed; newspaper and organizer returned; Week 1 autosave created. |
| Post-week manual save | PASS | Slot 002 named B; file B.02 created, 1,215,888 bytes. |
| Close and relaunch | PASS | Window close ended the process without a forced termination; game relaunched. |
| Load after restart | PASS | Selected B from initial Load Game menu; post-week newspaper restored. Hit the Streets displayed Week 2. |

No original game files or test saves are distributed. Save sizes are evidence for
this run, not required sizes for other games. This is a compatibility test, not a
validation of changes to gameplay rules. Multiplayer and additional PCs were not
part of this test.
