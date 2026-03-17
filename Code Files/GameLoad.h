#ifndef GameLoad_H
#define GameLoad_H

#include "Game.h"
class GameLoad : public Game
{
public:
	virtual void run(int mode)override;

	virtual int Play(int diff_lvl, Board& board, int score, std::string& file_results, std::string& file_steps)override;
};

#endif // !GameLoad_H

