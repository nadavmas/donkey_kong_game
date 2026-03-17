#ifndef Helper_h
#define Helper_h

#include <windows.h>
#include <iostream>
#include <conio.h>
using namespace std;
#include "Board.h"

// The Helpers module provides utility functions that assist in various gameplay operations.
// These functions include console cursor management, displaying lives, setting sleep durations, and managing game configurations.

class Board;

// Moves the console cursor to the specified position (x, y)
// Implementation was inspiered by example shown in the classroom
void gotoxy(int x, int y);

// Toggles the visibility of the console cursor
// Implementation was inspiered by example shown in the classroom
void ShowConsoleCursor(bool showFlag);

// Displays the player's remaining lives visually on the console
void showLives(int num_lives,location L);

// Displays the player's score visually on the console
void showScore(int score, location L);

// Sets the sleep duration between game cycles based on the difficulty level
void setSleep(int diff_lvl);

// Sets the number of iterations between each barrel spawn based on the difficulty level
void setBarrelQnt(int diff_lvl,int& barrelCount);

// Clears any remaining input from the console input buffer
// This function is used to prevent unwanted input from affecting the game state while mario is jumping or falling
void clearInputBuffer();



#endif

