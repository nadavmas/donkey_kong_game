# Donkey Kong Game (C++ Console)

Donkey Kong-style C++ console game with stage layouts loaded from `dkong*.screen` files.

## Quick Start

1. Open the Visual Studio solution: `Donkey_Kong_AN.sln`.
2. Build and run the startup project.
3. Note: stage discovery depends on the runtime working directory.

### Command-line modes

The entry point (`src/core/Main.cpp`) selects a mode based on arguments:

- Default: normal interactive run
- `-save`: record moves/results into `.steps` / `.result` files
- `-load`: replay a previously recorded run
- `-load -silent`: same as `-load` but with silent output mode

## Architecture

The game is split into a small “game runner” layer and the core stage loop:

```mermaid
flowchart TD
  Main[Main (src/core/Main.cpp)] --> Game[Game dispatch (src/core/Game.cpp)]
  Game --> Modes[Mode classes (GameSimple/GameSave/GameLoad/GameSilent)]
  Modes --> Engine[GameEngine::runStage (src/core/GameEngine.cpp)]
  Engine --> Board[Board (src/world/Board.cpp)]
  Engine --> Logic[Verifications (src/logic/Verifications.cpp)]
  Engine --> Entities[Mario/DK/Princess/Barrels/Ghosts]
  Save[SaveModeHooks] -->|record| StepsResults[Steps/Results files]
  Load[LoadModeHooks] -->|replay/check| StepsResults
```

## Code Structure

- `src/core/Main.cpp`
  - Parses CLI flags (`-save`, `-load`, `-silent`) and calls `game.run(mode)`.
- `src/core/Game.cpp`
  - Shows the main menu and dispatches to `GameSimple`, `GameSave`, `GameLoad`, or `GameSilent`.
  - Discovers stage files by scanning `std::filesystem::current_path()` (with `assets/` fallback).
- `src/core/GameEngine.cpp`
  - Contains `GameEngine::runStage(...)`: the shared per-stage loop (Mario/Barrels/Ghosts, collisions, difficulty).
  - Uses “mode hooks” (`GameModeHooks`) to decide how input/recording/rendering work.
- `src/world/Board.cpp`
  - Implements `Board::load(...)` to parse `.screen` and place entities/tiles.
- `src/logic/Verifications.cpp`
  - Collision and interaction helpers (ladder, hammer, falling checks, explosions, win condition).
- `src/objects/*`
  - `Mario`, `DonkeyKong`, `Princess`, `Barrel`, `Ghost` implementations.

## Stages and File Formats

### Stage discovery (runtime)

Stages are loaded by scanning the runtime current working directory for `dkong*.screen` (plus related `.steps`/`.result` files).

If nothing is found, the code falls back to looking for an `assets/` directory adjacent to the working directory.
See `src/core/Game.cpp`:

- `getAllBoardFileNames(...)` (`dkong*.screen`)
- `getAllStepsFileNames(...)` (`dkong*.steps`)
- `getAllResultFileNames(...)` (`dkong*.result`)

### `.screen` markers

`Board::load(...)` reads the file character-by-character and places entities based on marker tiles:

- `@`: Mario start
- `&`: Donkey Kong start
- `$`: Princess position
- `L`: legend top-left anchor (legend must fit within the board bounds)
- `x` / `X`: ghost spawn positions (two ghost types)
- `p`: hammer pickup position
- Extra life: a visual `"<3"` representation (the loader checks for a `3` whose previous character is `<`)

Additional tiles referenced by movement/collision logic include:

- `H`: ladder
- `Q`: walls/solid blocks
- `=`: platform/floor
- `<` / `>`: barrel ramp indicators

See `src/world/Board.cpp` for the entity placement rules and legend anchor handling.

## Controls (key actions)

Use the documented controls below:

- Move: `w`, `a`, `s`, `d`
  - After choosing a direction, Mario continues automatically.
  - Ladder behavior is special: pressing `s` when there is no ladder under Mario causes Mario to stay.
- Hammer: `p`
  - Hammer usage is enabled only when Mario has picked up the hammer.

Gameplay invariants (hammer pickup rules, extra-life rules, and `.screen` parsing expectations) are defined in `MEMORY_BANK.md`.

## Cursor Rules and `MEMORY_BANK.md`

This repo includes Cursor guidance files that act as “source of truth” for changes:

- `MEMORY_BANK.md`: project-specific invariants
- `.cursor/rules/project-context.mdc`: what to treat as truth before gameplay-affecting edits
- `.cursor/rules/changes-safety.mdc`: how to keep edits safe and reviewable
- `.cursor/rules/prompt-tracker.mdc`: how to log user prompts that result in file changes

If you change `.screen` parsing or any gameplay rules, update `MEMORY_BANK.md` in the same change so controls/tile legend/invariants remain consistent.

