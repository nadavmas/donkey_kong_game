# PROMPT_TRACKER

This file logs **USER prompts that resulted in file changes**.

Template per entry:

Prompt #N (Plan Mode/Agent Mode/Debug Mode):
"<the whole USER prompt>"

What Cursor did:
- <brief explanation of file changes>

---

Prompt #1 (Agent Mode):
"i want you to add a rule - i want that every agent\plan prompt that changes files in any way will be documented in a new \"PROMPT_TRACKER\" readme file. 
i want every prompt to be documented by the following template:
Prompt #(whatever this prompt's number is)(Plan Mode/Agent Mode):
\"the whole prompt\"

then explain what cursor did"

What Cursor did:
- Added `PROMPT_TRACKER.md` with the required logging template and first entry for this change-making prompt.
- Added a new Cursor rule at `.cursor/rules/prompt-tracker.mdc` to require appending a new entry whenever a USER prompt results in file changes (USER prompts only).

---

Prompt #2 (Agent Mode):
"Instead of having all of the files in code files, i want you to rearrange the files to folders in a more logical way that will help the repo being cleaner"

What Cursor did:
- Created a clean folder structure: `src/` (with `core/world/objects/ui/logic/utils`), `include/` (mirroring those areas), `assets/`, and `Donkey_Kong_AN/`.
- Moved source files out of `Code Files/` into the new folders; moved `.screen/.steps/.result` files into `assets/`.
- Moved Visual Studio project files into `Donkey_Kong_AN/` and solution to repo root.
- Updated `Donkey_Kong_AN.vcxproj` and `.filters` to reference new file paths, added include directories, and added a post-build step to copy `assets/dkong_*.screen` into `$(OutDir)` (next to the exe).
- Updated `MEMORY_BANK.md` to reflect the new structure and asset-copy behavior.

---

Prompt #3 (Agent Mode):
"delete @Assignment3_readme.txt.txt , its not relevant anymore"

What Cursor did:
- Deleted `Assignment3_readme.txt.txt`.
- Updated `MEMORY_BANK.md` to remove references to that deleted file.

---

Prompt #4 (Agent Mode):
"make sure that all changes related to the project bulding in visual studio arent included in the git repo using git ignore"

What Cursor did:
- Added `.gitignore` to exclude Visual Studio/VS Build artifacts (e.g., `.vs/`, `x64/`, `Debug/`, `*.user`, `*.pdb`, etc.) from the repo.

---

Prompt #5 (Debug Mode):
"theres a problem choosing difficulty when starting the game"

What Cursor did:
- Added temporary debug instrumentation to the menu and game flow to capture runtime behavior when selecting difficulty.
- Fixed stage discovery so `dkong_*.screen` files are found under `assets/` even when the working directory is different, and set the Visual Studio debugger working directory to `$(OutDir)`.
- Updated the level selection screen to show "Level 1", "Level 2", "Level 3" instead of full file paths.
- After verifying the fix, removed the debug instrumentation and its log file.

---

Prompt #7 (Plan Mode):
"I want to seperate the game engine from the game mode. propose the best 2 options to do so.  (this is the kind of prompts that i want to be logged into @PROMPT_TRACKER.md )"

What Cursor did:
- Proposed a refactor separation strategy by introducing a core `GameEngine` plus a mode strategy/callback interface (`GameModeHooks`), with each mode acting as a thin wrapper supplying input/recording/rendering behavior.
- Added the engine/mode separation implementation in `src/core/GameEngine.cpp` + corresponding hooks interface (and wired `GameSimple` to run a shared `runStage` loop via hooks).
- After separation, fixed stage discovery/difficulty selection by setting VS debugger working directory to `$(OutDir)`, adding `assets/` fallback scanning for `.screen` discovery, and updating the UI to show “Level 1/Level 2/Level 3”.
- After separation, fixed the “stuck in the air” player movement regression by restoring the keyboard-driven control flow inside the shared engine loop (including the inner `_kbhit()` wait loop and `clearInputBuffer()` timing / jump-fall reset logic).
- After separation, fixed Save-mode crash (`vector subscript out of range`) by ensuring save/load discovery uses the same robust `assets/` fallback scanning and adding NDJSON instrumentation for the out-of-range/indexing hypothesis.
- After separation, diagnosed the Load-mode infinite fast loop and Silent-mode failure by instrumenting the step-based loop control flow (NDJSON logs like `debug_stepBased_init`, `debug_stepBased_it_update`, `debug_stepBased_after_inner_no_sleep`) to confirm when the inner loop stops executing without outer-loop termination.

---

Prompt #8 (Plan Mode):
"I want you to create seperation between the game engine and the game modes using the first option you've offered: "Create one core GameEngine class that knows how to run a stage (the loop with Mario, barrels, ghosts, collisions, difficulty, etc.), and let each mode (GameSimple, GameSave, GameLoad, GameSilent) pass in a small strategy object or set of callbacks that decide how input/recording/rendering work.""

What Cursor did:
- Implemented the “one core `GameEngine` + strategy/callback hooks” approach by creating `GameEngine::runStage` as the shared gameplay loop and defining a `GameModeHooks`-style interface for mode-specific behavior (input source, whether to render, and step/result recording/checking).
- Wired `GameSimple` to call the engine’s shared stage loop through hooks, establishing the new engine/mode separation structure.
- After separation, fixed stage discovery/difficulty selection by using `$(OutDir)` as the debugger working directory, adding `assets/` fallback scanning for `.screen` discovery, and displaying “Level 1/Level 2/Level 3” instead of raw paths.
- After separation, fixed the “stuck in the air” movement regression by aligning the keyboard-mode loop behavior to the original `GameSimple::Play` control flow (inner `_kbhit()` wait loop, `clearInputBuffer()` placement/timing, and jump/fall reset logic).
- After separation, fixed Save-mode `vector subscript out of range` by making step/result file discovery robust (including `assets/` fallback scanning) and adding NDJSON logs to validate indexing assumptions.
- After separation, diagnosed the Load-mode infinite fast loop and Silent-mode not working by instrumenting the step-based loop (NDJSON logs like `debug_stepBased_init`, `debug_stepBased_it_update`, `debug_stepBased_after_inner_no_sleep`) to pinpoint loop termination behavior when steps become empty.

---

Prompt #9 (Plan Mode):
"I want you to create a readme for the project explaining the architechture, the structure, cursor rules and memory bank."

What Cursor did:
- Added `README.md` documenting the project architecture/structure plus links to `MEMORY_BANK.md` and the `.cursor/rules/*` guidance.



