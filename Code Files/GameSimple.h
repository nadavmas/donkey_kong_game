#ifndef GameSimple_H
#define GameSimple_H

#include "Game.h"

// The Game class manages the entire game flow, including menus, gameplay, and state transitions.
// It initializes the game, tracks the current state, and handles player interactions with the game logic.


// Enum to define the different game states



class GameSimple : public Game
{
public:
	// Constructor initializes the game as running
	GameSimple(): Game() {}


	// Starts the main game loop
	 // This function handles menu navigation, difficulty selection, and game sessions
	 void run(int mode) override;
	 

	// Handles the core gameplay logic
	// diff_lvl: The difficulty level selected by the player
	// Returns the state of the game after the session (WINNING, GAME_OVER, or EXIT)
	virtual int Play(int diff_lvl, Board& board, int score) override;
	//virtual int Play(int diff_lvl, Board& board, int score, std::string& file_results, std::string& file_steps)override;


};

#endif // !GameSimple

