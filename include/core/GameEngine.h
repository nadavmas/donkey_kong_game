#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <string>
#include <vector>

#include "Board.h"
#include "Mario.h"
#include "DonkeyKong.h"
#include "Princess.h"
#include "Barrel.h"
#include "Ghost.h"
#include "Steps.h"
#include "Results.h"

// Strategy interface implemented by each game mode (simple/save/load/silent).
struct GameModeHooks {
    virtual ~GameModeHooks() = default;

    // Whether the engine should poll keyboard for input.
    virtual bool usesKeyboard() const = 0;

    // Called once before the stage loop starts.
    virtual void onStageStart(int difficulty,
                              Steps& steps,
                              Results& results) = 0;

    // Provide the next input for this frame (keyboard or from Steps).
    virtual char getNextInput(const Steps& steps,
                              size_t iterationCount,
                              char currentInput) = 0;

    // Called when a player input is recorded (used by save/load modes).
    virtual void onStepRecorded(int iterationCount,
                                char input,
                                Steps& steps) = 0;

    // Called when a notable result occurs (win/die/useHammer etc.).
    virtual void onResultRecorded(int iterationCount,
                                  Results::ResultValue result,
                                  Results& results) = 0;

    // Rendering control.
    virtual bool shouldRender() const = 0;
    virtual void renderFrame(Board& board,
                             Mario& mario,
                             DonkeyKong& dk,
                             Princess& pr) = 0;
};

// Core gameplay engine: runs one stage using the provided mode hooks.
class GameEngine {
public:
    int runStage(int difficulty,
                 Board& board,
                 int score,
                 GameModeHooks& hooks,
                 std::string* resultsFile = nullptr,
                 std::string* stepsFile = nullptr);
};

#endif

