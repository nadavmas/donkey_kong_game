#ifndef GameSave_H
#define GameSave_H

#include "Game.h"

class GameSave : public Game
{
public:
	GameSave() : Game() {}
	
	//void run() = 0;
	void run(int mode) override;



	// Handles the core gameplay logic
	// diff_lvl: The difficulty level selected by the player
	// Returns the state of the game after the session (WINNING, GAME_OVER, or EXIT)
	//virtual int Play(int diff_lvl, Board& board, int score) override;
	virtual int Play(int diff_lvl, Board& board, int score, std::string& file_results, std::string& file_steps) override;
};
#endif // !GameSave_H
