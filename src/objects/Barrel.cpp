#include "Barrel.h"

// Simulates the explosion of a barrel
// The explosion affects a radius of 2 tiles around the barrel
void Barrel::explode(const Board& board) {
	
	const int radius = 2; // Explosion radius

	// Get the barrel's current position
	int bx = getX();
	int by = getY();

	// Store the explosion area for restoring later
	char explosionArea[(radius * 2) + 1][(radius * 2) + 1];

	// Calculate and draw the explosion
	for (int dy = -radius; dy <= radius; dy++) {
		for (int dx = -radius; dx <= radius; dx++) {
			int x = bx + dx;
			int y = by + dy;

			// Skip if out of bounds
			if (x < 1 || x >= 80 || y < 1 || y >= 25) {
				continue;
			}

			// Store the original character in the explosion area
			explosionArea[dy + radius][dx + radius] = board.getChar(x, y);

			// Draw explosion if the tile is empty or a ladder
			if (board.getChar(x, y) == ' ' || board.getChar(x, y) == 'H') {
				gotoxy(x, y);
				std::cout << '*'; // Explosion effect
			}
		}
	}
	
	Sleep(80); // Pause to visualize the explosion

	// Redraw the original objects in the explosion area
	for (int dy = -radius; dy <= radius; dy++) {
		for (int dx = -radius; dx <= radius; dx++) {
			int x = bx + dx;
			int y = by + dy;

			// Skip if out of bounds
			if (x < 1 || x >= 80 || y < 1 || y >= 25) {
				continue;
			}

			// Restore the original character
			gotoxy(x, y);
			std::cout << explosionArea[dy + radius][dx + radius];
		}
	}
}

// Moves the barrel based on its direction and state
void Barrel:: move(const Board& board) {
	int x = getX(), y = getY();
	if (movingDirection == LEFT && alive == true) {
		gotoxy(x, y);
		remove(board);
		setX(x - 1); // Move left
		setY(y);
		draw();
	}
	if (movingDirection == RIGHT && alive == true) {
		gotoxy(x, y);
		remove(board);
		setX(x + 1); // Move right
		setY(y);
		draw();
	}
	if (movingDirection == DOWN && alive == true) {
		gotoxy(x, y);
		remove(board); 
		setX(x);
		setY(y + 1); // Move down
		draw();
	}

	// Deactivate barrel if it reaches the bottom corners
	if ((getX() == 1 && getY() == 23) || (getX() == 77 && getY() == 23))
		alive = false;
}

void Barrel:: deactivate(const Board& board) {
	remove(board);
	setX(board.getDK().x-1);
	setY(board.getDK().y);
	fallCount = 0;
	alive = false;
}