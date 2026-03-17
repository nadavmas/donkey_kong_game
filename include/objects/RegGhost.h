#ifndef GhostReg_h
#define GhostReg_h

#include <iostream>
#include <random>
#include <ctime>

#include "GameObject.h"
#include "Board.h"
#include "Ghost.h"

// Regular ghost class that inherits from the Ghost class
// Moves only sideways
class RegGhost : public Ghost
{
	// Function to randomly decide the direction the ghost should move
	virtual int chooseState(int curr, int alter,Board& board, std::mt19937 gen);

public:
	RegGhost(location l) : Ghost(l, 'x') {}

	virtual void move(Board& board, std::mt19937 gen);

};
#endif 

