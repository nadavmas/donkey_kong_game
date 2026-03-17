#ifndef SupGhost_H
#define SupGhost_H

#include "Ghost.h"
#include "Board.h"
#include "Steps.h"

// Super ghost class that inherits from the Ghost class
// Moves in all directions (when theres a ladder)
class SupGhost : public Ghost
{
	bool isAboveladder(const Board& board) {
		if (board.getChar(this->getX(), this->getY() + 2) == 'H') {
			return true;
		}
		return false;

	}
	bool isUnderLadder(const Board& board) {
		if (board.getChar(this->getX(),this->getY() -1) == 'H') {
			return true;
		}
		return false;
	}
	bool isOnLadder(const Board& board) {
		// Saves the characters above and below Mario's position for comparison
		char above = board.getChar(this->getX(), this->getY() - 1);
		char under = board.getChar(this->getX(), this->getY() + 1);
		char under2 = board.getChar(this->getX(), this->getY() + 2);
		// Checking all cases in which Mario is on or interacting with a ladder
		if ((above == 'H' && under == 'H')) // Mario is on a ladder
			return true;
		else if (above == 'H' && (movingDirection == UP)) // Mario is climbing up a ladder
			return true;
		else if (under == 'H' && above == '=') // Mario is on a ladder
			return true;
		else if (under == '=' && under2 == 'H')
		{
			if (movingDirection == DOWN) // Mario is climbing down a ladder
				return true;
		}
		return false;
	}
	virtual int chooseState(int curr, int alter, Board& board, std::mt19937 gen);
	
public:

	SupGhost(location l) : Ghost(l, 'X') {}

	virtual void move(Board& board, std::mt19937 gen) ;
};
#endif // !SupGhost_H
