#ifndef Ghost_h
#define Ghost_h

#include <iostream>
#include <random>
#include <ctime>

#include "GameObject.h"
#include "Board.h"


class Ghost : public GameObject
{
protected:
	int movingDirection; // Direction in which the ghost is moving
	bool activated;
	// Function to randomly decide the direction the ghost should move
	virtual int chooseState(int curr, int alter,Board& board, std::mt19937 gen) = 0;
public:
	// Constructor initializes the ghost's attributes
	Ghost(location l,char type): GameObject(type, l), movingDirection(LEFT),activated(true) {}
	
	// Gets the moving direction of the ghost
	int getMovingDirection() const {
		return movingDirection;
	}

	bool getActivation() const {
		return activated;
	}

	void die(const Board &board) {
		activated = false;
		remove(board);
	}

	// Sets the moving direction of the ghost
	void setMovingDirection(int direction) {
		movingDirection = direction;
	}

    // Moves the ghost based on its direction
	virtual void move(Board& board, std::mt19937 gen) = 0;

    // Changes the direction of the ghost
	void changeDirection() {
		if (movingDirection == LEFT)
			movingDirection = RIGHT; // Switch to the right direction
		else
			movingDirection = LEFT; // Switch to the left direction
	}
};



#endif 

