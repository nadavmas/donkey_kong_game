#ifndef Barrel_h
#define Barrel_h

#include "GameObject.h"
#include "Helpers.h"
#include "Board.h"

// The Barrel class represents the barrels in the game that move and interact with the game environment.
// Barrels are activated by Donkey Kong and can move, fall, or explode based on interactions.

class Barrel : public GameObject
{
	int movingDirection; // Direction in which the barrel is moving
	int fallCount; // Tracks how long the barrel has been falling
	bool alive; // Indicates whether the barrel is active or not
public:
	// Constructor initializes the barrel's attributes
	Barrel(location l) :GameObject('O',l),movingDirection(LEFT),fallCount(0), alive(false) {}

	// Gets the fall count of the barrel
	int getFallCount() const
	{
		return fallCount;
	}

	// Gets the moving direction of the barrel
	int getMovingDirection() const {
		return movingDirection;
	}

	// Increments the fall count
	void setFallCount() {
		fallCount++;
	}

	// Resets the fall count to zero
	void resetFallCount() {
		fallCount = 0;
	}

	// Sets the moving direction of the barrel
	void setMovingDirection(int direction) {
		movingDirection = direction;
	}

	// Sets the barrel's coordinates
	void setCoor(int x, int y) {
		setX(x);
		setY(y);
	}

	// Returns the active state of the barrel
	bool getState() {
		return alive;
	}

	// Activates the barrel and sets its initial position
	void activate(const Board& board) {
		alive = true;
		setX(board.getDK().x-1);
		setY(board.getDK().y);
	}

	// Deactivates the barrel, removes it from the board, and resets its state
	void deactivate(const Board& board);

	// Handles the movement of the barrel
	void move(const Board& board);

	// Handles the explosion effect of the barrel
	// Implementation was made with the help of chatGPT
	void explode(const Board& board);

	
};

#endif 

