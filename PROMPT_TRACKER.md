# PROMPT_TRACKER

This file logs **USER prompts that resulted in file changes**.

Template per entry:

Prompt #N (Plan Mode/Agent Mode):
"the whole prompt"

What Cursor did:
- <brief explanation of file changes>

---

Prompt #1 (Agent Mode):
"Add PROMPT_TRACKER logging rule

Implement the plan as specified, it is attached for your reference. Do NOT edit the plan file itself.

To-do's from the plan have already been created. Do not create them again. Mark them as in_progress as you work, starting with the first one. Don't stop until you have completed all the to-dos."

What Cursor did:
- Added `PROMPT_TRACKER.md` with the required logging template and first entry for this change-making prompt.
- Added a new Cursor rule at `.cursor/rules/prompt-tracker.mdc` to require appending a new entry whenever a USER prompt results in file changes (USER prompts only).

