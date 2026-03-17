#ifndef Menu_h
#define Menu_h

#include <iostream>
#include <vector>
#include <limits>

#include "Helpers.h"
using namespace std;

// The Menu class manages the game's main menu, pause menu, and messages displayed to the player.
// It provides options to start the game, select difficulty levels, and handle game over or winning scenarios.

class Menu {
public: 
	// Displays the main menu with options to start the game or exit
	void printMenu();

	// Reads the player's choice from the input
   // Ensures only valid choices (1 or 2) are accepted
	int getChoice();

	// Displays a message when the player wins and provides options to replay or exit
	int winningMessage();

	int stageFinished();


	// Pauses the game and provides an option to continue or exit
	int pause();

	// Displays a game-over message and provides options to replay or exit
	int gameOverMessage();

	// Displays a difficulty selection menu and returns the player's choice
	int difficultyLevel();

	// Displays an error message when loading a board fails
	void errorInLoading();

	// Displays the level selection menu and gets the user's choice for a level
	int gameFlow(const vector<string>& files);


};

#endif 
