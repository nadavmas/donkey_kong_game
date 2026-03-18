#include "GameEngine.h"
#include "Game.h"

#include "Helpers.h"
#include "Menu.h"
#include "Verifications.h"
#include "RegGhost.h"
#include "SupGhost.h"
#include <conio.h>
#include <ctime>
#include <random>
#include <fstream>
#include <chrono>

// #region agent log
static void agentLog_15106d_frame(size_t iteration,
                                  char input,
                                  int walkingDirection,
                                  bool usesKeyboard,
                                  bool keyPressed)
{
    using namespace std;
    const auto ts = chrono::duration_cast<chrono::milliseconds>(
        chrono::system_clock::now().time_since_epoch()).count();
    ofstream f("debug-15106d.log", ios::app);
    if (!f.is_open()) return;
    f << "{\"sessionId\":\"15106d\",\"runId\":\"pre-fix\",\"hypothesisId\":\"H_movement\","
      << "\"location\":\"src/core/GameEngine.cpp:loop\",\"message\":\"frame\","
      << "\"data\":{\"iteration\":" << iteration
      << ",\"input\":\"" << input
      << "\",\"walkingDirection\":" << walkingDirection
      << ",\"usesKeyboard\":" << (usesKeyboard ? "true" : "false")
      << ",\"keyPressed\":" << (keyPressed ? "true" : "false")
      << "},\"timestamp\":" << ts << "}\n";
}
// #endregion agent log

int GameEngine::runStage(int diff_lvl,
                         Board& board,
                         int score,
                         GameModeHooks& hooks,
                         std::string* resultsFile,
                         std::string* stepsFile)
{
    system("cls");
    board.reset();
    board.print();

    Mario mario(board.getMario());
    showLives(mario.getNumOfLives(), board.getL());
    showScore(score, board.getL());
    DonkeyKong dk(board.getDK());
    Princess pr(board.getPrincess());
    ShowConsoleCursor(false);

    Barrel barrels[13] = { Barrel(board.getDK()), Barrel(board.getDK()), Barrel(board.getDK()),
        Barrel(board.getDK()), Barrel(board.getDK()), Barrel(board.getDK()),
        Barrel(board.getDK()), Barrel(board.getDK()), Barrel(board.getDK()),
        Barrel(board.getDK()), Barrel(board.getDK()), Barrel(board.getDK()),
        Barrel(board.getDK()) };

    std::vector<std::unique_ptr<Ghost>> ghosts;
    for (location num : board.getGhostsx())
        ghosts.push_back(std::make_unique<RegGhost>(num));
    for (location num : board.getGhostsX())
        ghosts.push_back(std::make_unique<SupGhost>(num));

    int i = 1;
    int MariofallCount = 0;
    int barrelCount = 1;
    int choice = 1;
    char input = '0';
    size_t iterationCount = 0;

    Results results;
    Steps steps;
    hooks.onStageStart(diff_lvl, steps, results);
    if (steps.getRandomSeed() == 0) {
        steps.setRandomSeed(std::time(0));
    }
    std::mt19937 gen(steps.getRandomSeed());

    Menu m;

    if (hooks.usesKeyboard()) {
        // Keyboard-driven modes (Simple, Save): mirror GameSimple::Play exactly
        // for movement control flow, while delegating recording via hooks.
        bool isLifeTaken = false;

        while (mario.getNumOfLives() > 0)
        {
            // Pause the game if ESC key is pressed
            if (input == 27) {
                system("cls");
                choice = m.pause();
                if (choice == 1) {
                    system("cls");
                    gotoxy(0, 0);
                    board.print();
                    mario.draw();
                    dk.draw();
                }
                else {
                    return Game::EXIT;
                }
            }

            // Check for win condition (Mario reaching the Princess)
            if (isWinning(mario, pr)) {
                hooks.onResultRecorded((int)iterationCount, Results::finished, results);
                hooks.onStepRecorded((int)iterationCount, 's', steps); // save-mode parity
                return Game::WINNING;
            }

            // Handle extra life collection
            if (isLifeTaken == false) {
                isExtraLife(board, mario, isLifeTaken);
                showLives(mario.getNumOfLives(), board.getL());
            }

            // Check and handle barrel explosions
            for (int bi = 0; bi < 13; ++bi) {
                if (barrels[bi].getFallCount() >= 8) {
                    if (board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == 'Q'
                        || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '='
                        || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '>'
                        || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '>'
                        || barrels[bi].getY() + 1 == 24) {
                        barrels[bi].explode(board);
                        if (isInExplosian(mario.getX(), mario.getY(), barrels[bi].getX(), barrels[bi].getY())) {
                            mario.die(board);
                            hooks.onResultRecorded((int)iterationCount, Results::die, results);
                        }
                        showLives(mario.getNumOfLives(), board.getL());
                        barrels[bi].deactivate(board);
                    }
                }
            }

            // Spawn barrels periodically
            barrelCount--;
            if (barrelCount <= 0) {
                int k = 0;
                bool found = false;
                while (k < 13 && found == false) {
                    if (barrels[k].getState() == false) {
                        barrels[k].activate(board);
                        found = true;
                    }
                    k++;
                }
                setBarrelQnt(diff_lvl, barrelCount);
            }

            // Check collisions with Donkey Kong
            if (isHittingDK(board, mario)) {
                mario.die(board);
                hooks.onResultRecorded((int)iterationCount, Results::die, results);
                showLives(mario.getNumOfLives(), board.getL());
                dk.draw();
                mario.setWalkingDirection(stay_);
            }

            // Check collisions with barrels
            if (isHittingBarrel(board, mario, barrels)) {
                mario.die(board);
                hooks.onResultRecorded((int)iterationCount, Results::die, results);
                showLives(mario.getNumOfLives(), board.getL());
                mario.setWalkingDirection(stay_);
            }

            if (isOnHammer(board, mario))
                mario.activateHammer(board);

            if (isHittingGhost(board, mario, ghosts)) {
                mario.die(board);
                hooks.onResultRecorded((int)iterationCount, Results::die, results);
                showLives(mario.getNumOfLives(), board.getL());
                mario.setWalkingDirection(stay_);
            }

            // Handle Mario's falling logic
            if (fallingCheck(board, mario) && mario.isJumping() == false) {
                clearInputBuffer();
                mario.setFalling(true);
                MariofallCount++;
                mario.setWalkingDirectionWhenFalling(board);
                mario.move(board);
                Sleep(100);
            }
            else {
                mario.setFalling(false);
                if (MariofallCount > 0) {
                    if (mario.getWalkingDirection() == DOWN)
                        mario.setWalkingDirection('s');
                    else if (mario.getWalkingDirection() == DOWN_RIGHT)
                        mario.setWalkingDirection('d');
                    else if (mario.getWalkingDirection() == DOWN_LEFT)
                        mario.setWalkingDirection('a');
                }

                if (MariofallCount >= 5) {
                    mario.die(board);
                    hooks.onResultRecorded((int)iterationCount, Results::die, results);
                    showLives(mario.getNumOfLives(), board.getL());
                    MariofallCount = 0;
                }
                else {
                    MariofallCount = 0;
                }

                // If a jump has ended, reset Mario's state
                if (i == (mario.jump_height * 2) + 1)
                {
                    mario.setJumping(false);
                    i = 1;
                    mario.jump_height = default_jump_height;
                    clearInputBuffer();
                    if (fallingCheck(board, mario))
                        mario.setFalling(true);
                }

                // Wait until a new key is pressed (or Mario starts jumping)
                while (!_kbhit() && mario.isJumping() == false && mario.getNumOfLives() > 0)
                {
                    // Check for win condition (Mario reaching the Princess)
                    if (isWinning(mario, pr)) {
                        hooks.onResultRecorded((int)iterationCount, Results::finished, results);
                        hooks.onStepRecorded((int)iterationCount, 's', steps);
                        return Game::WINNING;
                    }

                    // Handle extra life collection
                    if (isLifeTaken == false) {
                        isExtraLife(board, mario, isLifeTaken);
                        showLives(mario.getNumOfLives(), board.getL());
                    }

                    // Check and handle barrel explosions
                    for (int bi = 0; bi < 13; ++bi) {
                        if (barrels[bi].getFallCount() >= 8) {
                            if (board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == 'Q'
                                || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '='
                                || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '>'
                                || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '>'
                                || barrels[bi].getY() + 1 == 24) {
                                barrels[bi].explode(board);
                                if (isInExplosian(mario.getX(), mario.getY(), barrels[bi].getX(), barrels[bi].getY())) {
                                    mario.die(board);
                                    hooks.onResultRecorded((int)iterationCount, Results::die, results);
                                }
                                showLives(mario.getNumOfLives(), board.getL());
                                barrels[bi].deactivate(board);
                            }
                        }
                    }

                    // Spawn barrels periodically
                    barrelCount--;
                    if (barrelCount <= 0) {
                        int k = 0;
                        bool found = false;
                        while (k < 13 && found == false) {
                            if (barrels[k].getState() == false) {
                                barrels[k].activate(board);
                                found = true;
                            }
                            k++;
                        }
                        setBarrelQnt(diff_lvl, barrelCount);
                    }

                    // Check collisions with Donkey Kong
                    if (isHittingDK(board, mario)) {
                        mario.die(board);
                        hooks.onResultRecorded((int)iterationCount, Results::die, results);
                        showLives(mario.getNumOfLives(), board.getL());
                        dk.draw();
                        mario.setWalkingDirection(stay_);
                    }

                    // Check collisions with barrels
                    if (isHittingBarrel(board, mario, barrels)) {
                        mario.die(board);
                        hooks.onResultRecorded((int)iterationCount, Results::die, results);
                        showLives(mario.getNumOfLives(), board.getL());
                        mario.setWalkingDirection(stay_);
                    }

                    if (isOnHammer(board, mario))
                        mario.activateHammer(board);

                    if (isHittingGhost(board, mario, ghosts)) {
                        mario.die(board);
                        hooks.onResultRecorded((int)iterationCount, Results::die, results);
                        showLives(mario.getNumOfLives(), board.getL());
                        mario.setWalkingDirection(stay_);
                    }

                    // Handle Mario's falling logic inside the input-wait loop
                    if (fallingCheck(board, mario) && mario.isJumping() == false) {
                        clearInputBuffer();
                        mario.setFalling(true);
                        MariofallCount++;
                        mario.setWalkingDirectionWhenFalling(board);
                        mario.move(board);
                        Sleep(100);
                    }
                    else {
                        mario.setFalling(false);
                        if (MariofallCount > 0) {
                            if (mario.getWalkingDirection() == DOWN)
                                mario.setWalkingDirection('s');
                            else if (mario.getWalkingDirection() == DOWN_RIGHT)
                                mario.setWalkingDirection('d');
                            else if (mario.getWalkingDirection() == DOWN_LEFT)
                                mario.setWalkingDirection('a');
                        }

                        if (MariofallCount >= 5) {
                            mario.die(board);
                            hooks.onResultRecorded((int)iterationCount, Results::die, results);
                            showLives(mario.getNumOfLives(), board.getL());
                            MariofallCount = 0;
                        }
                        else {
                            MariofallCount = 0;
                        }

                        // Handle wall collisions
                        if (isHittingWall(board, mario)) {
                            input = mario.getWalkingDirection();
                        }

                        if (mario.getWalkingDirection() == UP || mario.getWalkingDirection() == DOWN) {
                            if (isOnLadder(board, mario, input) == false)
                                mario.setWalkingDirection(stay_);
                        }

                        mario.move(board);
                        setSleep(diff_lvl);
                    }

                    // If Mario starts falling again, set falling state
                    if (fallingCheck(board, mario))
                        mario.setFalling(true);

                    // Check barrel collisions again after movement
                    if (isHittingBarrel(board, mario, barrels)) {
                        mario.die(board);
                        hooks.onResultRecorded((int)iterationCount, Results::die, results);
                        showLives(mario.getNumOfLives(), board.getL());
                        mario.setWalkingDirection(stay_);
                    }

                    ghostsDirections(ghosts);
                    moveBarrel(board, barrels);
                    moveGhost(board, ghosts, gen);

                    iterationCount++;
                }

                if (mario.getNumOfLives() == 0)
                    break;

                // Handle player input
                if (mario.isJumping() == false)
                    input = _getch();

                // Allow hooks to adjust mapping (SaveModeHooks is identity today)
                input = hooks.getNextInput(steps, iterationCount, input);

                // Record step for save-mode replay (simple-mode no-op)
                if (mario.isJumping() == false) {
                    if (input == 'w' || input == 'W')
                        hooks.onStepRecorded((int)iterationCount, 'w', steps);
                    else if (input == 'a' || input == 'A')
                        hooks.onStepRecorded((int)iterationCount, 'a', steps);
                    else if (input == 'd' || input == 'D')
                        hooks.onStepRecorded((int)iterationCount, 'd', steps);
                    else if (input == 's' || input == 'S')
                        hooks.onStepRecorded((int)iterationCount, 's', steps);
                    else if (input == 'p' || input == 'P')
                        hooks.onStepRecorded((int)iterationCount, 'p', steps);
                }

                if ((input == 'P' || input == 'p') && mario.isHoldingHammerF()) {
                    hooks.onResultRecorded((int)iterationCount, Results::useHammer, results);
                    mario.useHammer(board, ghosts, barrels);
                }
                else if (input == 'w' || input == 'W')
                {
                    if (!isOnLadder(board, mario, input)) {
                        if (i == 1) {
                            int k = 1;
                            while (k <= default_jump_height) {
                                if (mario.getWalkingDirection() == STAY)
                                    if (board.getChar(mario.getX(), mario.getY() - k) == '=' || board.getChar(mario.getX(), mario.getY() - k) == 'Q') {
                                        mario.jump_height = k - 1;
                                        break;
                                    }
                                if (board.getChar(mario.getX() - (k * pow(-1, mario.getWalkingDirection())), mario.getY() - k) == '='
                                    || board.getChar(mario.getX(), mario.getY() - k) == 'Q') {
                                    mario.jump_height = k - 1;
                                    break;
                                }
                                k++;
                            }
                        }
                        if (mario.isFalling() == false) {
                            if (mario.jump_height > 0) {
                                mario.setJumping(true);
                                mario.jump(board, i);
                                Sleep(100);
                                i++;
                            }
                            else {
                                mario.jump_height = default_jump_height;
                            }
                        }
                    }
                    else
                        mario.setWalkingDirection(input);
                }
                else if (input == 's' || input == 'S')
                {
                    if (mario.isFalling() == false) {
                        mario.setWalkingDirection(input);
                        if (!isOnLadder(board, mario, input))
                            mario.setWalkingDirection(stay_);
                    }
                }
                else {
                    if (!isOnLadder(board, mario, input))
                        mario.setWalkingDirection(input);
                }

                // If Mario starts falling again, set falling state
                if (fallingCheck(board, mario))
                    mario.setFalling(true);

                // Final check for collisions with barrels after movement
                if (isHittingBarrel(board, mario, barrels)) {
                    mario.die(board);
                    hooks.onResultRecorded((int)iterationCount, Results::die, results);
                    showLives(mario.getNumOfLives(), board.getL());
                    mario.setWalkingDirection(stay_);
                }

                ghostsDirections(ghosts);
                moveBarrel(board, barrels);
                moveGhost(board, ghosts, gen);
                iterationCount++;
                if (hooks.shouldRender()) {
                    hooks.renderFrame(board, mario, dk, pr);
                }
            }
        }

        return Game::GAME_OVER;
    }

    // Step-based modes (Load, Silent): mirror GameLoad::Play control flow.
    // hooks.getNextInput provides the next input at the same logical ticks
    // where the old code would have popped from Steps.
    bool isLifeTaken = false;
    int it = steps.getSteps().empty() ? 0 : steps.getNextStep();
    int debugStepEmptyLogs = 0;

    // #region debug_stepBased_init
    {
        const auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
        std::ofstream f("debug-15106d.log", std::ios::app);
        if (f.is_open()) {
            const bool stepsEmpty = steps.getSteps().empty();
            f << "{\"sessionId\":\"15106d\",\"runId\":\"debug_stepBased_init\",\"hypothesisId\":\"H_load_loop_speed\",\"location\":\"GameEngine.cpp:stepBased:init\","
              << "\"message\":\"init\",\"data\":{\"it\":" << it
              << ",\"stepsEmpty\":" << (stepsEmpty ? "true" : "false")
              << ",\"iterationCount\":"
              << iterationCount << "},\"timestamp\":" << ts << "}\n";
        }
    }
    // #endregion

    while (mario.getNumOfLives() > 0)
    {
        // Pause the game if ESC key is pressed
        if (input == 27) {
            system("cls");
            choice = m.pause();
            if (choice == 1) {
                system("cls");
                gotoxy(0, 0);
                board.print();
                mario.draw();
                dk.draw();
            }
            else {
                return Game::EXIT;
            }
        }

        // Check for win condition (Mario reaching the Princess)
        if (isWinning(mario, pr)) {
            // Load/Silent reference implementations do: curr_result.first++;
            // LoadModeHooks/SilentModeHooks do not apply this offset internally,
            // so we mirror it by shifting the iteration passed to the hook.
            int adjusted = (iterationCount > 0) ? (int)(iterationCount - 1) : 0;
            hooks.onResultRecorded(adjusted, Results::finished, results);
            return Game::WINNING;
        }

        // Handle extra life collection
        if (isLifeTaken == false) {
            isExtraLife(board, mario, isLifeTaken);
            showLives(mario.getNumOfLives(), board.getL());
        }

        // Check and handle barrel explosions
        for (int bi = 0; bi < 13; ++bi) {
            if (barrels[bi].getFallCount() >= 8) {
                if (board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == 'Q'
                    || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '='
                    || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '>'
                    || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '>'
                    || barrels[bi].getY() + 1 == 24) {
                    barrels[bi].explode(board);
                    if (isInExplosian(mario.getX(), mario.getY(), barrels[bi].getX(), barrels[bi].getY())) {
                        mario.die(board);
                        hooks.onResultRecorded((int)iterationCount, Results::die, results);
                    }
                    showLives(mario.getNumOfLives(), board.getL());
                    barrels[bi].deactivate(board);
                }
            }
        }

        // Spawn barrels periodically
        barrelCount--;
        if (barrelCount <= 0) {
            int k = 0;
            bool found = false;
            while (k < 13 && found == false) {
                if (barrels[k].getState() == false) {
                    barrels[k].activate(board);
                    found = true;
                }
                k++;
            }
            setBarrelQnt(steps.getDifficulty(), barrelCount);
        }

        // Check collisions with Donkey Kong
        if (isHittingDK(board, mario)) {
            mario.die(board);
            hooks.onResultRecorded((int)iterationCount, Results::die, results);
            dk.draw();
            showLives(mario.getNumOfLives(), board.getL());
            mario.setWalkingDirection(stay_);
        }

        // Check collisions with barrels
        if (isHittingBarrel(board, mario, barrels)) {
            mario.die(board);
            hooks.onResultRecorded((int)iterationCount, Results::die, results);
            showLives(mario.getNumOfLives(), board.getL());
            mario.setWalkingDirection(stay_);
        }

        if (isOnHammer(board, mario))
            mario.activateHammer(board);

        if (isHittingGhost(board, mario, ghosts)) {
            mario.die(board);
            hooks.onResultRecorded((int)iterationCount, Results::die, results);
            showLives(mario.getNumOfLives(), board.getL());
            mario.setWalkingDirection(stay_);
        }

        // Handle Mario's falling logic
        if (fallingCheck(board, mario) && mario.isJumping() == false) {
            clearInputBuffer();
            mario.setFalling(true);
            MariofallCount++;
            mario.setWalkingDirectionWhenFalling(board);
            mario.move(board);
            Sleep(100);
        }
        else {
            mario.setFalling(false);
            if (MariofallCount > 0) {
                if (mario.getWalkingDirection() == DOWN)
                    mario.setWalkingDirection('s');
                else if (mario.getWalkingDirection() == DOWN_RIGHT)
                    mario.setWalkingDirection('d');
                else if (mario.getWalkingDirection() == DOWN_LEFT)
                    mario.setWalkingDirection('a');
            }

            if (MariofallCount >= 5) {
                mario.die(board);
                hooks.onResultRecorded((int)iterationCount, Results::die, results);
                showLives(mario.getNumOfLives(), board.getL());
                mario.setWalkingDirection(stay_);
                MariofallCount = 0;
            }
            else {
                MariofallCount = 0;
            }

            if (i == (mario.jump_height * 2) + 1)
            {
                mario.setJumping(false);
                i = 1;
                mario.jump_height = default_jump_height;
                clearInputBuffer();
                if (fallingCheck(board, mario))
                    mario.setFalling(true);
            }

            bool innerExecuted = false;
            while (iterationCount < (size_t)it && mario.isJumping() == false && mario.getNumOfLives() > 0)
            {
                innerExecuted = true;
                if (isWinning(mario, pr)) {
                    int adjusted = (iterationCount > 0) ? (int)(iterationCount - 1) : 0;
                    hooks.onResultRecorded(adjusted, Results::finished, results);
                    return Game::WINNING;
                }

                if (isLifeTaken == false) {
                    isExtraLife(board, mario, isLifeTaken);
                    showLives(mario.getNumOfLives(), board.getL());
                }

                for (int bi = 0; bi < 13; ++bi) {
                    if (barrels[bi].getFallCount() >= 8) {
                        if (board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == 'Q'
                            || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '='
                            || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '>'
                            || board.getChar(barrels[bi].getX(), barrels[bi].getY() + 1) == '>'
                            || barrels[bi].getY() + 1 == 24) {
                            barrels[bi].explode(board);
                            if (isInExplosian(mario.getX(), mario.getY(), barrels[bi].getX(), barrels[bi].getY())) {
                                mario.die(board);
                                hooks.onResultRecorded((int)iterationCount, Results::die, results);
                            }
                            showLives(mario.getNumOfLives(), board.getL());
                            barrels[bi].deactivate(board);
                        }
                    }
                }

                barrelCount--;
                if (barrelCount <= 0) {
                    int k = 0;
                    bool found = false;
                    while (k < 13 && found == false) {
                        if (barrels[k].getState() == false) {
                            barrels[k].activate(board);
                            found = true;
                        }
                        k++;
                    }
                    setBarrelQnt(steps.getDifficulty(), barrelCount);
                }

                if (isHittingDK(board, mario)) {
                    mario.die(board);
                    hooks.onResultRecorded((int)iterationCount, Results::die, results);
                    dk.draw();
                    showLives(mario.getNumOfLives(), board.getL());
                    mario.setWalkingDirection(stay_);
                }

                if (isHittingBarrel(board, mario, barrels)) {
                    mario.die(board);
                    hooks.onResultRecorded((int)iterationCount, Results::die, results);
                    showLives(mario.getNumOfLives(), board.getL());
                    mario.setWalkingDirection(stay_);
                }

                if (isOnHammer(board, mario))
                    mario.activateHammer(board);

                if (isHittingGhost(board, mario, ghosts)) {
                    mario.die(board);
                    hooks.onResultRecorded((int)iterationCount, Results::die, results);
                    showLives(mario.getNumOfLives(), board.getL());
                    mario.setWalkingDirection(stay_);
                }

                if (fallingCheck(board, mario) && mario.isJumping() == false) {
                    clearInputBuffer();
                    mario.setFalling(true);
                    MariofallCount++;
                    mario.setWalkingDirectionWhenFalling(board);
                    mario.move(board);
                    Sleep(100);
                }
                else {
                    mario.setFalling(false);
                    if (MariofallCount > 0) {
                        if (mario.getWalkingDirection() == DOWN)
                            mario.setWalkingDirection('s');
                        else if (mario.getWalkingDirection() == DOWN_RIGHT)
                            mario.setWalkingDirection('d');
                        else if (mario.getWalkingDirection() == DOWN_LEFT)
                            mario.setWalkingDirection('a');
                    }

                    if (MariofallCount >= 5) {
                        mario.die(board);
                        hooks.onResultRecorded((int)iterationCount, Results::die, results);
                        showLives(mario.getNumOfLives(), board.getL());
                        mario.setWalkingDirection(stay_);
                        MariofallCount = 0;
                    }
                    else {
                        MariofallCount = 0;
                    }

                    if (isHittingWall(board, mario)) {
                        input = mario.getWalkingDirection();
                    }

                    if (mario.getWalkingDirection() == UP || mario.getWalkingDirection() == DOWN) {
                        if (isOnLadder(board, mario, input) == false)
                            mario.setWalkingDirection(stay_);
                    }

                    mario.move(board);
                    setSleep(steps.getDifficulty());
                }

                if (fallingCheck(board, mario))
                    mario.setFalling(true);

                if (isHittingBarrel(board, mario, barrels)) {
                    mario.die(board);
                    hooks.onResultRecorded((int)iterationCount, Results::die, results);
                    showLives(mario.getNumOfLives(), board.getL());
                    mario.setWalkingDirection(stay_);
                }

                ghostsDirections(ghosts);
                moveBarrel(board, barrels);
                moveGhost(board, ghosts, gen);
                iterationCount++;
            }

            if (mario.getNumOfLives() == 0)
                break;

            if (mario.isJumping() == false) {
                input = hooks.getNextInput(steps, iterationCount, input);
                // NOTE: This mirrors legacy control flow, but we log it to validate
                // whether setting `it` to 0 when steps are exhausted is causing
                // the outer loop to run without setSleep/movement.
                it = steps.getSteps().empty() ? 0 : steps.getNextStep();

                // #region debug_stepBased_it_update
                {
                    const auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();
                    std::ofstream f("debug-15106d.log", std::ios::app);
                    if (f.is_open()) {
                        f << "{\"sessionId\":\"15106d\",\"runId\":\"debug_stepBased_it_update\",\"hypothesisId\":\"H_load_loop_speed\",\"location\":\"GameEngine.cpp:stepBased:itUpdate\","
                          << "\"message\":\"itUpdate\",\"data\":{\"innerExecuted\":" << (innerExecuted ? "true" : "false")
                          << ",\"iterationCount\":" << iterationCount
                          << ",\"it\":" << it
                          << ",\"stepsEmpty\":" << (steps.getSteps().empty() ? "true" : "false")
                          << "},\"timestamp\":" << ts << "}\n";
                    }
                }
                // #endregion
            }

            // #region debug_stepBased_after_inner_no_sleep
            if (!innerExecuted && debugStepEmptyLogs < 5 && steps.getSteps().empty()) {
                debugStepEmptyLogs++;
                const auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch()).count();
                std::ofstream f("debug-15106d.log", std::ios::app);
                if (f.is_open()) {
                    f << "{\"sessionId\":\"15106d\",\"runId\":\"debug_stepBased_after_inner_no_sleep\",\"hypothesisId\":\"H_load_loop_speed\",\"location\":\"GameEngine.cpp:stepBased:noInnerExecuted\","
                      << "\"message\":\"stepsExhausted_noInner\",\"data\":{\"iterationCount\":" << iterationCount
                      << ",\"it\":" << it
                      << ",\"marioJumping\":" << (mario.isJumping() ? "true" : "false")
                      << ",\"numLives\":" << mario.getNumOfLives()
                      << "},\"timestamp\":" << ts << "}\n";
                }
            }
            // #endregion

            if ((input == 'P' || input == 'p') && mario.isHoldingHammerF()) {
                hooks.onResultRecorded((int)iterationCount, Results::useHammer, results);
                mario.useHammer(board, ghosts, barrels);
            }
            else if (input == 'w' || input == 'W')
            {
                if (!isOnLadder(board, mario, input)) {
                    if (i == 1) {
                        int k = 1;
                        while (k <= default_jump_height) {
                            if (mario.getWalkingDirection() == STAY)
                                if (board.getChar(mario.getX(), mario.getY() - k) == '=' || board.getChar(mario.getX(), mario.getY() - k) == 'Q') {
                                    mario.jump_height = k - 1;
                                    break;
                                }
                            if (board.getChar(mario.getX() - (k * pow(-1, mario.getWalkingDirection())), mario.getY() - k) == '='
                                || board.getChar(mario.getX(), mario.getY() - k) == 'Q') {
                                mario.jump_height = k - 1;
                                break;
                            }
                            k++;
                        }
                    }
                    if (mario.isFalling() == false)
                    {
                        if (mario.jump_height > 0) {
                            mario.setJumping(true);
                            mario.jump(board, i);
                            Sleep(100);
                            i++;
                        }
                        else
                            mario.jump_height = default_jump_height;
                    }
                }
                else
                    mario.setWalkingDirection(input);
            }
            else if (input == 's' || input == 'S')
            {
                if (mario.isFalling() == false) {
                    mario.setWalkingDirection(input);
                    if (!isOnLadder(board, mario, input))
                        mario.setWalkingDirection(stay_);
                }
            }
            else {
                if (!isOnLadder(board, mario, input))
                    mario.setWalkingDirection(input);
            }
        }

        // Final check for collisions with barrels after movement
        if (isHittingBarrel(board, mario, barrels)) {
            mario.die(board);
            hooks.onResultRecorded((int)iterationCount, Results::die, results);
            showLives(mario.getNumOfLives(), board.getL());
            mario.setWalkingDirection(stay_);
        }

        ghostsDirections(ghosts);
        moveBarrel(board, barrels);
        moveGhost(board, ghosts, gen);
        iterationCount++;

        if (hooks.shouldRender()) {
            hooks.renderFrame(board, mario, dk, pr);
        }
    }

    return Game::GAME_OVER;
}

