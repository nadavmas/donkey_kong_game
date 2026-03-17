#include "Verifications.h"

// Checks if Mario is colliding with Donkey Kong
bool isHittingDK(const Board& const board, Mario& mario) {
	if (mario.getX() == board.getDK().x && mario.getY() == board.getDK().y)
		return true; // Returns true if Mario's position matches Donkey Kong's position
	return false;
}

// Checks if Mario is falling 
bool fallingCheck(const Board& board, Mario& mario) {
	bool isAboveObject = false;
	if(board.getChar(mario.getX(), mario.getY() + 1) == 'x')
		isAboveObject = true;
	if(board.getChar(mario.getX(), mario.getY() + 1) == 'o')
		isAboveObject = true;
	if(board.getChar(mario.getX(), mario.getY() + 1) == 'p')
		isAboveObject = true;
	if (board.getChar(mario.getX(), mario.getY() + 1) == '&')
		isAboveObject = true;
	if (board.getChar(mario.getX(), mario.getY() + 1) == '$')
		isAboveObject = true;
	if (board.getChar(mario.getX(), mario.getY() + 1) == '3')
		isAboveObject = true;
	if (board.getChar(mario.getX(), mario.getY() + 1) == '<')
	{
		if (board.getChar(mario.getX()+1, mario.getY() + 1) == '3')
			isAboveObject = true;
	}
	if ((board.getChar(mario.getX(), mario.getY() + 1) == ' '|| isAboveObject ==true) && mario.getY()+1!=24)
		return true; // Returns true if there is no solid ground below Mario
	return false;
}

// Checks if Mario is colliding with any active barrel
// Deactivates the barrel and decreases Mario's lives if a collision occurs
bool isHittingBarrel(const Board& board, Mario mario, Barrel barrels[13]) {
	for (int i = 0; i < 13; i++)
		if (mario.getX() == barrels[i].getX() && mario.getY() == barrels[i].getY()) {
			barrels[i].deactivate(board); // Deactivates the barrel if it collides with Mario
			mario.die(board); // Kills Mario if it collides with a barrel
			return true; // Returns true if Mario collides with a barrel
		}
	return false;
}

// Checks if Mario is colliding with a wall
// Adjusts Mario's direction if a wall is detected
bool isHittingWall(const Board& board, Mario& mario) {
	if (mario.getWalkingDirection() == LEFT) {
		if (board.getChar(mario.getX() - 1, mario.getY()) == 'Q')
		{
			mario.setWalkingDirection('d'); // Change direction from left to right if Mario hits the left wall
			return true;
		}
	}
	if (mario.getWalkingDirection() == RIGHT) {
		if (board.getChar(mario.getX() + 1, mario.getY()) == 'Q')
		{
			mario.setWalkingDirection('a'); // Change direction from right to left if Mario hits the right wall
			return true;
		}
	}
	if (mario.getWalkingDirection() == UP) {

		if (board.getChar(mario.getX(), mario.getY() - 1) == 'Q')
			return true; // Returns true if Mario hits a wall while moving up, without changing direction
	}
	if (mario.getWalkingDirection() == DOWN) {
		if (board.getChar(mario.getX(), mario.getY() + 1) == 'Q')
			return true; // Returns true if Mario hits a wall while moving down, without changing direction
	}
	return false; // Returns false if Mario does not hit a wall

}

// Checks if a position (mx, my) is within the radius of a barrel explosion
bool isInExplosian(int mx, int my, int bx, int by) {
	//calculates the radius of the explosion, which is 2 blocks in each direction and checks if the position (mx, my) is within the radius
	for (int i = bx - 2; i <= bx + 2; i++) {
		if (mx == i && my == by + 2)
			return true; 
	}
	for (int i = bx - 2; i <= bx + 2; i++) {
		if (mx == i && my == by + 1)
			return true;
	}
	for (int i = bx - 2; i <= bx + 2; i++) {
		if (mx == i && my == by)
			return true;
	}
	for (int i = bx - 2; i <= bx + 2; i++) {
		if (mx == i && my == by - 1)
			return true;
	}
	for (int i = bx - 2; i <= bx + 2; i++) {
		if (mx == i && my == by - 2)
			return true;
	}
	return false; // Returns false if the position is not within the explosion radius
}

// Checks if Mario is on or interacting with a ladder
bool isOnLadder(const Board& board, Mario& mario, char input)
{
	// Saves the characters above and below Mario's position for comparison
	char above = board.getChar(mario.getX(), mario.getY() - 1);
	char under = board.getChar(mario.getX(), mario.getY() + 1);
	char under2 = board.getChar(mario.getX(), mario.getY() + 2);
	// Checking all cases in which Mario is on or interacting with a ladder
	if ((above == 'H' && under == 'H')) // Mario is on a ladder
		return true;
	else if (above == 'H' && (input == 'w' || input == 'W')) // Mario is climbing up a ladder
		return true;
	else if (under == 'H' && above == '=') // Mario is on a ladder
		return true;
	else if (under == '=' && under2 == 'H') 
	{
		if (mario.getWalkingDirection() == DOWN) // Mario is climbing down a ladder
			return true;
	}
	return false;
}

// Checks if Mario is on the hammer
// Returns true if Mario is on the hammer and the hammer hasn't been taken
bool isOnHammer(const Board& board, Mario& mario) {
	// Check if Mario's position matches the position of the hammer on the board
	if (mario.getX() == board.getHammer().x && mario.getY() == board.getHammer().y && !board.getHammerTaken()) {
		return true; // Mario is on the hammer and it hasn't been taken
	}

	return false; // Returns false if Mario is not on the hammer or if it has already been taken
}

// Handles the movement of barrels
// Updates their direction and handles falling behavior
void moveBarrel(const Board& board, Barrel(&barrels)[13]) {
	// Iterate over all barrels
	for (int i = 0; i < 13; i++) {
		// Check if the tile below the barrel contains '>' (indicating a ramp to the right)
		if (board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == '>' && barrels[i].getState() == true)
			barrels[i].setMovingDirection(RIGHT); // Set the direction to right

		// Check if the tile below the barrel contains '<' (indicating a ramp to the left)
		if (board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == '<' && barrels[i].getState() == true)
			barrels[i].setMovingDirection(LEFT); // Set the direction to left

		// Check if the tile below the barrel is empty (' ')
		if (board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == ' ' && barrels[i].getState() == true) {
			if (barrels[i].getMovingDirection() == DOWN) // If the barrel is already falling, increase the fall count
				barrels[i].setFallCount();
			else {
				barrels[i].resetFallCount(); // Reset the fall count if it was previously moving horizontally
				barrels[i].setFallCount(); // Start a new fall count
			}
			barrels[i].setMovingDirection(DOWN); // Set the direction to down
		}

		// Check if the tile below the barrel contains 'Q' (indicating an obstacle or wall)
		if (board.getChar(barrels[i].getX()+1, barrels[i].getY()) == 'Q' && barrels[i].getState() == true&&barrels[i].getMovingDirection()==RIGHT)
			barrels[i].setMovingDirection(LEFT); // Set the direction to right
		if (board.getChar(barrels[i].getX() - 1, barrels[i].getY()) == 'Q' && barrels[i].getState() == true && barrels[i].getMovingDirection() == LEFT)
			barrels[i].setMovingDirection(RIGHT); // Set the direction to left

		// Move the barrel based on its current direction
		barrels[i].move(board);
	}
}

// Handles the movement of ghosts
// Updates their direction and moves them if they are activated
void moveGhost(Board& board, std::vector < std::unique_ptr<Ghost>>& ghosts, std::mt19937 gen) {
	// Iterate over each ghost in the vector
	for (std::unique_ptr<Ghost>& num : ghosts) {
		// If the ghost is activated (alive and able to move)
		if (num->getActivation()) {
			num->move(board, gen);
		}
	}
}

// Checks if Mario has reached the Princess to win the game
bool isWinning(Mario mario, Princess pr)
{
	if (mario.getX() == pr.getX() && mario.getY() == pr.getY())
		return true; // Returns true if Mario's position matches the Princess's position
	else
		return false;
}

// Checks if Mario has collected an extra life
void isExtraLife(const Board& board, Mario& mario, bool& isLifeTaken) {
	if ((mario.getX() == board.getLife().x - 1 && mario.getY() == board.getLife().y) || (mario.getX() == board.getLife().x && mario.getY() == board.getLife().y))
	{
		mario.addLife(); // Add an extra life to Mario
		isLifeTaken = true; // Mark that the extra life was taken

		// Clear the extra life icon from the board 
		// Its done by clearing the second part of the heart that mario collected (mario deleted the first part by moving to the first part)
		if (mario.getX() == board.getLife().x) {
			gotoxy(board.getLife().x-1, board.getLife().y);
			cout << ' ';
		}
		else {
			gotoxy(board.getLife().x, board.getLife().y);
			cout << ' ';
		}

	}
}

// Checks if Mario is colliding with any active ghost
// Returns true if a collision occurs (Mario dies)
bool isHittingGhost(const Board& board, Mario mario, std::vector<std::unique_ptr<Ghost>>& ghosts) {
	for (auto& num : ghosts) { // `num` is a unique pointer to a Ghost
		// Check if Mario's position matches the ghost's or if Mario is interacting with the ghost while not jumping
		if ((mario.getX() == num->getX() && mario.getY() == num->getY()) ||
			((mario.getX() == num->getX() && mario.getY() == num->getY() - 1) && mario.isJumping() == false) ||
			(mario.getWalkingDirection() == RIGHT && num->getMovingDirection() == LEFT && mario.getX() + 1 == num->getX() && mario.getY() == num->getY()) ||
			(mario.getWalkingDirection() == LEFT && num->getMovingDirection() == RIGHT && num->getX() + 1 == mario.getX() && num->getY() == mario.getY())) {

			if (num->getActivation()) { // Dereference to call `getActivation`
				mario.die(board); // Mario dies on collision
				return true;
			}
		}
	}
	return false;
}

// Checks if two ghosts are colliding
// Returns true if they are colliding, false otherwise
bool areGhostsColliding(const std::unique_ptr<Ghost>& a, const std::unique_ptr<Ghost>& b) {
	// Checks if the ghosts are at the same position or adjacent
	if ((a->getX() == b->getX() && a->getY() == b->getY()) ||
		(a->getY() == b->getY() && a->getX() == b->getX() + 1) ||
		(a->getY() == b->getY() && a->getX() == b->getX() - 1)) {
		return true;
	}
	return false;
}

// Changes direction of ghosts if they collide with each other
void ghostsDirections(std::vector < std::unique_ptr<Ghost>>& ghosts) {
	for (auto i = 0; i < ghosts.size(); i++) {
		for (auto j = i + 1; j < ghosts.size(); j++) {
			if (areGhostsColliding(ghosts[i], ghosts[j])) {
				ghosts[i]->changeDirection(); // Change direction of the first ghost
				ghosts[j]->changeDirection();
			}
		}
	}
}


