#include "Mario.h"

// Moves Mario based on the current walking direction
void Mario::move(Board& board) {
	int x = getX(), y = getY();
	if (walkingDirection == LEFT) {
		gotoxy(x, y);
		remove(board);
		setX(x - 1); // Move left
		setY(y);
		draw();
	}

	if (walkingDirection == RIGHT) {
		gotoxy(x, y);
		remove(board);
		setX(x + 1); // Move right
		setY(y);
		draw();
	}

	if (walkingDirection == DOWN) {

		gotoxy(x, y);
		remove(board);
		if (board.getChar(x, y + 1) == '=' && board.getChar(x, y + 2) == 'H')
		{
			setX(x);
			setY(y + 2); // Skip a ladder step if getting down a ladder from the floor
		}
		else
		{
			setX(x);
			setY(y + 1); // Move down
		}
		draw();
	}

	if (walkingDirection == UP) {
		gotoxy(x, y);
		remove(board);
		if (board.getChar(x, y - 1) == '=')
		{
			setX(x);
			setY(y - 2); // Skip a ladder step if getting to the next floor from the ladder
		}
		else
		{
			setX(x);
			setY(y - 1); // Move up
		}
		draw();
	}

	if (walkingDirection == DOWN_LEFT) {
		gotoxy(x, y);
		remove(board);
		setX(x - 1); // Move down-left (used when falling)
		setY(y + 1);
		draw();
	}

	if (walkingDirection == DOWN_RIGHT) {
		gotoxy(x, y);
		remove(board);
		setX(x + 1); // Move down-right (used when falling)
		setY(y + 1);
		draw();
	}


}


// Handles Mario's jump logic
// i: The current jump iteration
void Mario::jump(Board& board, int& i) {
	int new_x, new_y;

	// Determine the new x and y positions based on direction
	if (this->getWalkingDirection() == RIGHT)
		new_x = this->getX() + 1;
	else if (this->getWalkingDirection() == LEFT)
		new_x = this->getX() - 1;
	else
		new_x = this->getX();

	if (i <= jump_height)
		new_y = this->getY() - 1; // Move upward until reaching the jump height
	else
		new_y = this->getY() + 1; // Move downward after reaching the jump height
		
	// Check if the jump is interrupted by a ladder
	if (board.getChar(new_x, new_y) == 'H')
	{
		i = 0;
		is_jumping = false;
		this->setWalkingDirection(stay_); // Stops jumping and stying on the ladder
	}

	// Handle collisions with walls ('Q') during the jump
	if (this->getWalkingDirection() == LEFT)
	{
		if (board.getChar(new_x - 1, new_y) == 'Q')
		{
			this->setWalkingDirection('d'); // Change moving direction to right if hitting left wall while jumping
		}
	}
	if (this->getWalkingDirection() == RIGHT)
	{
		if (board.getChar(new_x + 1, new_y) == 'Q')
		{
			this->setWalkingDirection('a'); // Change moving direction to left if hitting right wall
		}
	}
	

	// Update Mario's position and redraw
	gotoxy(new_x, new_y);
	remove(board); // Remove Mario from the old position
	setX(new_x);       // Update Mario's X position
	setY(new_y);       // Update Mario's Y position
	draw();        // Draw Mario at the new position
}

// Updates Mario's moving direction when falling
void Mario::setWalkingDirectionWhenFalling(Board board) {
	if (walkingDirection == LEFT)
		// If there's a platform to the left while walking left, fall straight down
		if (board.getChar(getX()-1,getY()+1) == '=')
			this->walkingDirection = DOWN;
		else
			this->walkingDirection = DOWN_LEFT; // Otherwise, fall down-left
	if (walkingDirection == RIGHT)
		// If there's a platform to the right while walking right, fall straight down
		if (board.getChar(getX() + 1, getY() + 1) == '=')
			this->walkingDirection = DOWN;
		else
			this->walkingDirection = DOWN_RIGHT; // Otherwise, fall down-right
}

// Sets Mario's walking direction based on input
// wd: The direction ('w', 'a', 's', 'd', or stay_)
void Mario::setWalkingDirection(char wd) {
	if (wd == 'w' || wd == 'W')
		this->walkingDirection = UP;
	if (wd == 's' || wd == 'S')
		this->walkingDirection = DOWN;
	if (wd == 'a' || wd == 'A')
		this->walkingDirection = LEFT;
	if (wd == 'd' || wd == 'D')
		this->walkingDirection = RIGHT;
	if (wd == stay_)
		this->walkingDirection = STAY;
}

// Resets Mario to the starting position after losing a life
void Mario::die(const Board& board) {
	remove(board);// Remove Mario from the current position
	setSymbol('@'); // Reset Mario's symbol
	setX(board.getMario().x); // Reset x-coordinate
	setY(board.getMario().y); // Reset y-coordinate
	draw(); // Redraw Mario at the starting position
	lives--; // Decrease the number of lives
	is_falling = false; // Reset falling state
	isHoldingHammer = false; // Reset hammer state
	walkingDirection = STAY; // Reset walking direction
}

// Using the hammer to kill ghosts and deactivate barrels
void Mario::useHammer(Board& board, std::vector<std::unique_ptr<Ghost>>& ghosts, Barrel barrel[13]) {
	// Loop through each barrel
	for (int j = 0; j < 13; j++) {
		// Check if Mario is near a barrel based on walking direction
		if ((getWalkingDirection() == LEFT && getX() - 2 == barrel[j].getX() && getY() == barrel[j].getY()) ||
			(getWalkingDirection() == RIGHT && getX() + 2 == barrel[j].getX() && getY() == barrel[j].getY()) ||
			(getWalkingDirection() == LEFT && getX() - 1 == barrel[j].getX() && getY() == barrel[j].getY()) ||
			(getWalkingDirection() == RIGHT && getX() + 1 == barrel[j].getX() && getY() == barrel[j].getY())) {
			// Deactivate barrel if near Mario and in walking direction
			barrel[j].deactivate(board);
		}
	}

	// Loop through each ghost in the vector of unique_ptr<Ghost>
	for (auto& ghost : ghosts) {
		// Check if Mario is near a ghost based on walking direction
		if ((getWalkingDirection() == LEFT && getX() - 2 == ghost->getX() && getY() == ghost->getY()) ||
			(getWalkingDirection() == RIGHT && getX() + 2 == ghost->getX() && getY() == ghost->getY()) ||
			(getWalkingDirection() == LEFT && getX() - 1 == ghost->getX() && getY() == ghost->getY()) ||
			(getWalkingDirection() == RIGHT && getX() + 1 == ghost->getX() && getY() == ghost->getY())) {
			// Deactivate ghost if near Mario and in walking direction
			ghost->die(board); // Deactivate the ghost
		}
	}
}

