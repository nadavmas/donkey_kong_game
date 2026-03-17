# Donkey Kong Game — Memory Bank

This file is the durable, project-specific context Cursor should rely on when making changes.
Keep it short and high-signal; update it when gameplay rules, file formats, or run modes change.

## Project summary
- **Project type**: C++ console game (Visual Studio solution).
- **Primary goal**: A Donkey Kong–style platform game with stages loaded from `.screen` files.
- **Entry point**: `Code Files/Main.cpp` (calls `Game::run(mode)`).

## How to run
- **Build/Run (recommended)**: open `Code Files/Donkey_Kong_AN.sln` in Visual Studio and run the startup project.
- **Runtime working directory matters**:
  - The game discovers stage/steps/results files by scanning `std::filesystem::current_path()` for files named `dkong*.screen`, `dkong*.steps`, `dkong*.result` (see `Code Files/Game.cpp`).
  - Ensure the **working directory** contains those files (or set VS “Debugging → Working Directory” accordingly).
- **Command-line modes** (see `Code Files/Main.cpp`):
  - **Default**: no args → normal interactive run
  - **Save mode**: `-save`
  - **Load mode**: `-load`
  - **Load + silent mode**: `-load -silent`

## Player controls & movement rules
From `Assignment3_readme.txt.txt`:
- **Movement**: `w`, `a`, `s`, `d`
  - After choosing a direction, **Mario continues automatically** in that direction.
  - Pressing `s` when there is **no ladder under Mario** causes Mario to **stay**.
- **Hammer action**: `p` to hit with the hammer (when holding it).

## Gameplay invariants (do not break without explicit request)
From `Assignment3_readme.txt.txt` + code behavior:
- **Hammer availability**:
  - Hammer exists as a single pickup on a board (loaded from `.screen`).
  - Hammer can be taken only once per stage run (tracked by `Board::isHammerTaken`).
  - If Mario dies after taking the hammer, he **cannot pick it up again** unless the stage is restarted/reset (matches Assignment 2 note).
- **Extra life is stage-defined**:
  - Extra-life location is loaded from the `.screen` file (not hardcoded).
  - When collected once, it is removed from view for that stage run.

## Stage file discovery and locations
- **On disk in this repo**: `.screen` files currently live under `Code Files/`:
  - `Code Files/dkong_01.screen`
  - `Code Files/dkong_02.screen`
  - `Code Files/dkong_03.screen`
- **At runtime**: the game loads by scanning the **current working directory** for `dkong*.screen` and related files (see `Code Files/Game.cpp`).

## `.screen` board markers (file format hints)
Loaded in `Code Files/Board.cpp` (`Board::load`):
- **`@`**: Mario start
- **`&`**: Donkey Kong start
- **`$`**: Princess position
- **`L`**: Legend top-left anchor (legend must fit inside board)
- **`x` / `X`**: ghost spawn positions (two ghost types tracked separately)
- **`p`**: hammer pickup position
- **Extra life**: represented visually as `"<3"`; loader keys off the `'3'` character and confirms the previous char is `'<'`

Other notable tiles referenced in movement/collision code (`Code Files/Verifications.cpp`, `Code Files/Mario.cpp`):
- **`H`**: ladder
- **`Q`**: walls/solid blocks
- **`=`**: platform/floor
- **`<` / `>`**: ramp indicators used by barrel movement

## Key code landmarks
- **File discovery**: `Code Files/Game.cpp` (`getAllBoardFileNames`, `getAllStepsFileNames`, `getAllResultFileNames`)
- **Board parsing**: `Code Files/Board.cpp` (`Board::load`)
- **Collision/movement checks**: `Code Files/Verifications.cpp`
- **Mario movement/jump/hammer**: `Code Files/Mario.cpp` and `Code Files/Mario.h`
- **Mode routing**: `Code Files/Main.cpp`

## When to update this memory bank
Update `MEMORY_BANK.md` when you change:
- Controls, auto-move behavior, ladder rules, jump rules, hammer behavior, lives behavior
- `.screen` parsing or tile legend
- How the game discovers/loads stage files (especially anything involving working directory)
- Command-line modes / save-load behavior

