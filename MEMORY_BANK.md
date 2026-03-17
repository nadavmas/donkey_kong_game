# Donkey Kong Game — Memory Bank

This file is the durable, project-specific context Cursor should rely on when making changes.
Keep it short and high-signal; update it when gameplay rules, file formats, or run modes change.

## Project summary
- **Project type**: C++ console game (Visual Studio solution).
- **Primary goal**: A Donkey Kong–style platform game with stages loaded from `.screen` files.
- **Entry point**: `src/core/Main.cpp` (calls `Game::run(mode)`).

## How to run
- **Build/Run (recommended)**: open `Donkey_Kong_AN.sln` in Visual Studio and run the startup project.
- **Runtime working directory matters**:
  - The game discovers stage/steps/results files by scanning `std::filesystem::current_path()` for files named `dkong*.screen`, `dkong*.steps`, `dkong*.result` (see `src/core/Game.cpp`).
  - This repo keeps stage files under `assets/`, and the Visual Studio project is configured to **copy `assets/dkong_*.screen` next to the built executable** after build (post-build step), so running from VS should find them in `$(OutDir)`.
- **Command-line modes** (see `src/core/Main.cpp`):
  - **Default**: no args → normal interactive run
  - **Save mode**: `-save`
  - **Load mode**: `-load`
  - **Load + silent mode**: `-load -silent`

## Player controls & movement rules
- **Movement**: `w`, `a`, `s`, `d`
  - After choosing a direction, **Mario continues automatically** in that direction.
  - Pressing `s` when there is **no ladder under Mario** causes Mario to **stay**.
- **Hammer action**: `p` to hit with the hammer (when holding it).

## Gameplay invariants (do not break without explicit request)
From assignment requirements + code behavior:
- **Hammer availability**:
  - Hammer exists as a single pickup on a board (loaded from `.screen`).
  - Hammer can be taken only once per stage run (tracked by `Board::isHammerTaken`).
  - If Mario dies after taking the hammer, he **cannot pick it up again** unless the stage is restarted/reset (matches Assignment 2 note).
- **Extra life is stage-defined**:
  - Extra-life location is loaded from the `.screen` file (not hardcoded).
  - When collected once, it is removed from view for that stage run.

## Stage file discovery and locations
- **On disk in this repo**: stage files live under `assets/`:
  - `assets/dkong_01.screen`
  - `assets/dkong_02.screen`
  - `assets/dkong_03.screen`
- **At runtime**: the game loads by scanning the **current working directory** for `dkong*.screen` and related files (see `src/core/Game.cpp`).

## `.screen` board markers (file format hints)
Loaded in `src/world/Board.cpp` (`Board::load`):
- **`@`**: Mario start
- **`&`**: Donkey Kong start
- **`$`**: Princess position
- **`L`**: Legend top-left anchor (legend must fit inside board)
- **`x` / `X`**: ghost spawn positions (two ghost types tracked separately)
- **`p`**: hammer pickup position
- **Extra life**: represented visually as `"<3"`; loader keys off the `'3'` character and confirms the previous char is `'<'`

Other notable tiles referenced in movement/collision code (`src/logic/Verifications.cpp`, `src/objects/Mario.cpp`):
- **`H`**: ladder
- **`Q`**: walls/solid blocks
- **`=`**: platform/floor
- **`<` / `>`**: ramp indicators used by barrel movement

## Key code landmarks
- **File discovery**: `src/core/Game.cpp` (`getAllBoardFileNames`, `getAllStepsFileNames`, `getAllResultFileNames`)
- **Board parsing**: `src/world/Board.cpp` (`Board::load`)
- **Collision/movement checks**: `src/logic/Verifications.cpp`
- **Mario movement/jump/hammer**: `src/objects/Mario.cpp` and `include/objects/Mario.h`
- **Mode routing**: `src/core/Main.cpp`

## When to update this memory bank
Update `MEMORY_BANK.md` when you change:
- Controls, auto-move behavior, ladder rules, jump rules, hammer behavior, lives behavior
- `.screen` parsing or tile legend
- How the game discovers/loads stage files (especially anything involving working directory)
- Command-line modes / save-load behavior

