#ifndef Mario_h
#define Mario_h


#include <conio.h>
#include <Windows.h>
#include <chrono>
#include <thread>

#include "GameObject.h"
#include "Helpers.h"
#include "Board.h"
#include "Ghost.h"
#include "Barrel.h"

// The Mario class represents the player-controlled character in the game.
// Mario can move, jump, fall, and interact with the game environment.

static const int default_jump_height = 2; // Default height Mario can jump
static const char stay_ = 'x'; // Represents no movement



class Mario : public GameObject
{
	int lives; // Number of lives Mario has
	int walkingDirection; // Current movement direction
	bool is_jumping; // Indicates if Mario is jumping
	bool is_falling; // Indicates if Mario is falling
	bool isHoldingHammer = false; // Indicates if Mario is holding a hammer
public:
	int jump_height = 2; // Current jump height
	// Constructor initializes Mario with 3 lives and the symbol '@'
	Mario(location l) : GameObject('@',l), lives(3), walkingDirection(STAY), is_jumping(false), is_falling(false) {
		draw(); // Draw Mario at the initial position
	}

	// Returns Mario's current walking direction
	int getWalkingDirection() const {
		return walkingDirection;
	}
	bool isHoldingHammerF() const {
		return isHoldingHammer;
	}

	// Returns the number of lives Mario has
	int getNumOfLives() const {
		return this->lives;
	}


	// Returns true if Mario is falling
	bool isFalling() const {
		return is_falling;
	} 

	// Returns true if Mario is jumping
	bool isJumping() const {
		return is_jumping;
	} 

	// Decreases Mario's lives by 1
	void setMarioslives() {
		this->lives--;
	}

	// Sets the falling state of Mario
	void setFalling(bool set) {
		is_falling = set;
	}

	// Sets the jumping state of Mario
	void setJumping(bool set) {
		is_jumping = set;
	}

	// Adds one life to Mario
	void addLife(){
		this->lives++;
	}

	// Activates the hammer for Mario, allowing it to be used
	void activateHammer(Board& board) {
		board.setHammerTaken(true); // Mark the hammer as taken on the board
		isHoldingHammer = true; // Set Mario as holding the hammer
		setSymbol('P'); // Change Mario's symbol to 'P' to indicate he is holding the hammer
	}

	// Sets Mario's walking direction based on input
	// wd: The direction ('w', 'a', 's', 'd', or stay_)
	void setWalkingDirection(char wd);


	// Resets Mario to the starting position after losing a life
	void die(const Board& board);

	// Moves Mario based on the current direction
	void move(Board& board);

	// Handles Mario's jump logic
	// i: The current jump iteration
	void jump(Board& board, int& i);

	// Sets the moving direction while Mario is falling
	void setWalkingDirectionWhenFalling(Board board);

	// Using the hammer to kill ghosts and deactivate barrels
	void useHammer(Board& board, std::vector<std::unique_ptr<Ghost>>& ghosts, Barrel barrel[13]);

};

#endif


