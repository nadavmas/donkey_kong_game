#include <cstring>
#include <iostream>
#include <fstream>

#include "Board.h"
#include "location.h"


// Resets the board to its initial state by copying the originalBoard to the currentBoard
void Board::reset() {
	for (int i = 0; i < MAX_Y; i++) {
		// Copy each row of the original board to the current board
		memcpy(currentBoard[i], originalBoard[i], MAX_X + 1);
	}
}

// Prints the current state of the board to the console
void Board::print() const {
	for (int i = 0; i < MAX_Y; i++) {
		for (int j = 0; j < MAX_X; j++) {
			std::cout << currentBoard[i][j];
		}
		std::cout << std::endl;
	}
	
}


// Inserts a legend (visual elements) onto the board, like the 'life' and score labels
void Board::insertLegend() {
	int counter = 0;
	// Loop through the horizontal space for the legend and place 'Q' characters (likely for borders)
	for (int i = L.x; i < L.x + 31; i++) {
		originalBoard[L.y][i] = 'Q'; // Top border of the legend
		originalBoard[L.y + 2][i] = 'Q'; // Bottom border of the legend
		if (counter <= 2) {
			originalBoard[L.y + counter][L.x] = 'Q'; // Left border of the legend
			originalBoard[L.y + counter][L.x + 31] = 'Q'; // Right border of the legend
			counter++;
		}
	}

	// Insert the labels for 'Life' and 'Score' in the appropriate spots
	originalBoard[L.y + 1][L.x + 1] = 'l';
	originalBoard[L.y + 1][L.x + 2] = 'i';
	originalBoard[L.y + 1][L.x + 3] = 'f';
	originalBoard[L.y + 1][L.x + 4] = 'e';
	originalBoard[L.y + 1][L.x + 5] = ':';

	originalBoard[L.y + 1][L.x + 20] = 's';
	originalBoard[L.y + 1][L.x + 21] = 'c';
	originalBoard[L.y + 1][L.x + 22] = 'o';
	originalBoard[L.y + 1][L.x + 23] = 'r';
	originalBoard[L.y + 1][L.x + 24] = 'e';
	originalBoard[L.y + 1][L.x + 25] = ':';
}

// Loads a board from a file and initializes the positions of Mario, Donkey Kong, Princess, and other objects
int Board::load(const std::string& filename) {

    std::ifstream screen_file(filename); // Open the screen file
	if (!screen_file.is_open()) {
		std::cout << "Error: Could not open screen file " << filename << std::endl;
		return BAD; // Return error if the file couldn't be opened
	}

    int curr_row = 0;
    int curr_col = 0;
    char c;

    // Read the file character by character
    while (!screen_file.get(c).eof() && curr_row < MAX_Y) {
        if (c == '\n') {
            if (curr_col < MAX_X) {
                return BAD; // Return error if the line doesn't have enough characters
            }
            curr_row++; // Move to the next row
            curr_col = 0; // Reset column to the beginning
        }
        else {
            // Set the positions of Mario, Donkey Kong, Princess, etc.
            if (c == '@') {
                if (mario.x != -1)
                    return BAD; // Error if Mario's position is already set
                mario.x = curr_col;
                mario.y = curr_row;
            }
            if (c == '&') {
                if (dk.x != -1)
                    return BAD; // Error if Donkey Kong's position is already set
                dk.x = curr_col;
                dk.y = curr_row;
            }
            if (c == '$') {
                if (princess.x != -1)
                    return BAD; // Error if Princess's position is already set
                princess.x = curr_col;
                princess.y = curr_row;
            }
            if (c == 'L') {
                if (L.x != -1)
                    return BAD; // Error if the legend position is already set
                L.x = curr_col;
                L.y = curr_row;
            }
            if (c == 'x') {
                // Add the position of a ghost to the ghosts list
                ghostsxL.push_back(location{ curr_col, curr_row });
            }
            if (c == 'X') {
                // Add the position of a ghost to the ghosts list
                ghostsXL.push_back(location{ curr_col, curr_row });
            }
            if (c == 'p') {
                if (hammer.x != -1)
                    return BAD; // Error if the hammer's position is already set
                hammer.x = curr_col;
                hammer.y = curr_row;
            }
            if (c == '3') {
                if (life.x != -1)
                    return BAD; // Error if the extra life position is already set
                if (originalBoard[curr_row][curr_col - 1] == '<') {
                    life.x = curr_col;
                    life.y = curr_row;
                }
            }
            originalBoard[curr_row][curr_col] = c; // Set the character in the board array
            curr_col++; // Move to the next column
        }
    }

    // Check if the legend fits within the board boundaries
    if (L.x + 30 > MAX_X)
        return BAD;
    if (L.y + 2 > MAX_Y)
        return BAD;
    if (curr_row < MAX_Y - 1)
        return BAD;

    insertLegend(); // Add the legend to the board after loading
    return GOOD; // Return success if everything loaded correctly
}