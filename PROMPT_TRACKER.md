# PROMPT_TRACKER

This file logs **USER prompts that resulted in file changes**.

Template per entry:

Prompt #N (Plan Mode/Agent Mode):
"the whole prompt"

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



