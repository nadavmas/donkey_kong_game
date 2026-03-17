#ifndef Game_h
#define Game_h

#include <iostream>
#include <conio.h>
#include <Windows.h>
#include <math.h>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <filesystem>


#include "barrel.h"
#include "Board.h"
#include "DonkeyKong.h"
#include "GameObject.h"
#include "Helpers.h"
#include "Mario.h"
#include "Princess.h"
#include "Menu.h"
#include "Verifications.h"
#include "Results.h"
#include "Steps.h"

// The Game class manages the entire game flow, including menus, gameplay, and state transitions.
// It initializes the game, tracks the current state, and handles player interactions with the game logic.


// Enum to define the different game states



class Game
{
public:
		// Enum to define the different game states
		enum GameState {
			GAME_OVER, // Indicates the game has ended due to Mario running out of lives
			WINNING,   // Indicates the player has won by reaching the Princess
			EXIT	   // Indicates the game was exited by the player
		};
protected:
	vector <string> screenfiles;
	GameState state; // Stores the current state of the game
	bool is_running; // Indicates whether the game loop is active

public:
	class innerEnum {
		// Enum to define the different game states
		enum GameState {
			GAME_OVER, // Indicates the game has ended due to Mario running out of lives
			WINNING,   // Indicates the player has won by reaching the Princess
			EXIT	   // Indicates the game was exited by the player
		};
	};
	// Constructor initializes the game as running
	Game() : is_running(true) {}
	
	// Returns whether the game loop is currently active
	bool getIsRunning() {
		return is_running;
	}
	
	// Sets whether the game loop should be active or not
	void setIsRunning(bool set) {
		is_running = set;
	}

	// Starts the main game loop
	 // This function handles menu navigation, difficulty selection, and game sessions
	virtual void run(int mode);

	void getAllBoardFileNames(std::vector<std::string>& vec_to_fill) const;
	void getAllStepsFileNames(std::vector<std::string>& vec_to_fill) const;
	void getAllResultFileNames(std::vector<std::string>& vec_to_fill) const;

	// Handles the core gameplay logic
	// diff_lvl: The difficulty level selected by the player
	// Returns the state of the game after the session (WINNING, GAME_OVER, or EXIT)
	virtual int Play(int diff_lvl,Board &board,int score);
	virtual int Play(int diff_lvl, Board& board, int score, std::string& file_results,std::string& file_steps);

	

};

#endif