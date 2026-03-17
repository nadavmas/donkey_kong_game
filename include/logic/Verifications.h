#ifndef Verifications_h
#define Verifications_h


#include "Helpers.h"
#include "Mario.h"
#include "Board.h"
#include "Barrel.h"
#include "Princess.h"
#include "Ghost.h"
#include "SupGhost.h"
#include "RegGhost.h"
#include "Results.h"
#include "Steps.h"
#include <conio.h>
#include <vector>

// The Verifications module provides utility functions to handle various checks and interactions
// in the game. These include collision detection, winning conditions, and barrel movement.

// Checks if Mario is colliding with Donkey Kong
bool isHittingDK(const Board& board, Mario& mario);

// Checks if Mario is falling (no solid ground below)
bool fallingCheck(const Board& board, Mario& mario);

// Checks if Mario is colliding with any active barrel
bool isHittingBarrel(const Board& board, Mario mario, Barrel barrels[13]);

// Checks if Mario is colliding with a wall
bool isHittingWall(const Board& board, Mario& mario);

// Checks if a position (mx, my) is within the radius of a barrel explosion
bool isInExplosian(int mx, int my, int bx, int by);

// Checks if Mario is on a ladder or interacting with a ladder
bool isOnLadder(const Board& board, Mario& mario, char input);

// Checks if Mario is on the hammer
// Returns true if Mario is on the hammer and the hammer hasn't been taken
bool isOnHammer(const Board& board, Mario& mario);

// Handles the movement of barrels, including direction changes and falling
void moveBarrel(const Board& board, Barrel(&barrels)[13]);

// Handles the movement of ghosts
// Updates their direction and moves them if they are activated
void moveGhost(Board& board, std::vector < std::unique_ptr<Ghost>>& ghosts, std::mt19937 gen);

// Checks if Mario has reached the Princess to win the game
bool isWinning(Mario mario, Princess pr);

// Checks if Mario has collected an extra life
void isExtraLife(const Board& board, Mario& mario, bool& isLifeTaken);

// Checks if Mario is colliding with any active ghost
// Returns true if a collision occurs (Mario dies)
bool isHittingGhost(const Board& board, Mario mario, std::vector<std::unique_ptr<Ghost>>& ghosts);

// Checks if two ghosts are colliding
// Returns true if they are colliding, false otherwise
bool areGhostsColliding(const std::unique_ptr<Ghost>& a, const std::unique_ptr<Ghost>& b);

// Changes direction of ghosts if they collide with each other
void ghostsDirections(std::vector < std::unique_ptr<Ghost>>& ghosts);


#endif