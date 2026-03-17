#ifndef Board_h 
#define Board_h

#include "location.h"
#include <vector>

// The Board class manages the game map as a 2D grid and provides functionality
// for resetting and displaying the board during the game.
//
// This class's implementation was inspired by examples covered in class with Amir.
enum boardState { GOOD, BAD };

class Board {
	location mario;          // Stores Mario's position on the board
	location dk;             // Stores Donkey Kong's position
	location princess;       // Stores Princess's position
	location L;              // Location for the "legend" (labels like score and life)
	location hammer;         // Stores the position of the hammer
	bool isHammerTaken = false; // Flag to check whether Mario has already taken the hammer
	location life;           // Position of extra life on the board
	std::vector <location> ghostsxL; // List to store positions of all ghosts on the board
	std::vector <location> ghostsXL;
	static constexpr int MAX_X = 80; // Constant for the width of the board (80 columns)
	static constexpr int MAX_Y = 25; // Constant for the height of the board (25 rows)
	char currentBoard[MAX_Y][MAX_X + 1]; // 2D array for the current state of the board, including the null terminator for each row
	char originalBoard[MAX_Y][MAX_X + 1]; // 2D array to store the original state of the board, used to reset the board
	
public:
	// Resets the board to its original state 
	void reset();  

	// Prints the current state of the board to the console
	void print() const; 

	// Retrieves the character at a specific position on the board
	char getChar(int x, int y) const {
		return currentBoard[y][x]; 
	} 

	// Loads the board from a file and initializes positions of objects
   // Reads the file and places Mario, Donkey Kong, Princess, etc., at the correct positions
	int load(const std::string& filename);

	// Returns true if the hammer has been taken by Mario
	bool getHammerTaken() const {
		return isHammerTaken;
	}

	// Sets the hammer's taken status to true or false
	void setHammerTaken(bool b) {
		isHammerTaken = b;
	}

	// Inserts visual elements like score and life labels to the board
	// This method places the labels 'life:' and 'score:' at specific positions on the board
	void insertLegend();

	// Returns the current position of Mario
	location getMario() const {
		return mario;
	}

	// Returns the current position of the Princess
	location getPrincess() const {
		return princess;
	}

	// Returns the current position of Donkey Kong
	location getDK() const {
		return dk;
	}

	// Returns the current position of the hammer
	location getHammer() const {
		return hammer;
	}

	// Returns a list of all ghosts' positions
	std::vector<location> getGhostsx() const {
		return ghostsxL;
	}
	std::vector<location> getGhostsX() const {
		return ghostsXL;
	}
	// Returns the position of the legend section on the board
	location getL() const {
		return L;
	}

	// Returns the position of the extra life on the board
	location getLife() const {
		return life;
	}

	// Resets the locations of all objects (Mario, Princess, Donkey Kong, ghosts, etc.) to their default state
	void resetLocations() {
		mario.x = -1;
		mario.y = -1;
		princess.x = -1;
		princess.y = -1;
		dk.x = -1;
		dk.y = -1;
		hammer.x = -1;
		hammer.y = -1;
		L.x = -1;
		L.y = -1;
		life.x = -1;
		life.y = -1;
		ghostsxL.clear();
		ghostsXL.clear();// Clears the list of ghosts from the board
		isHammerTaken = false; // Resets the hammer taken flag
	}

};

#endif 
