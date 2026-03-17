#ifndef GameObject_h
#define GameObject_h

#include <iostream>
#include <conio.h>
#include <Windows.h>

#include "Helpers.h"
#include "Board.h"

// The GameObject class represents a generic object in the game, with a position (x, y)
// and a symbol used to display the object on the game board.

enum Directions { LEFT, RIGHT, UP, DOWN, STAY, DOWN_RIGHT, DOWN_LEFT };

class GameObject
{
    int x, y; // Coordinates of the object
    char symbol; // Symbol representing the object on the board
private:
    // Draws the object using the specified character 
    void draw(char c) {
        gotoxy(x, y); // Move cursor to (x, y)
        std::cout << c; // Display the character
    }

public:
  

    // Constructor initializes the object with a symbol and sets its starting position
	GameObject(char symb,location l) : symbol(symb) {
        x = l.x;
        y = l.y;
		// Set the object's position based on the symbol
      
    }

    // Returns the x-coordinate of the object
    int getX() const { 
        return this->x; 
    } 

    // Returns the y-coordinate of the object
    int getY() const {
        return this->y;
    } 

    // Returns the symbol of the object
    char getSymbol() const {
        return this->symbol; 
    } 

    // Sets a new x-coordinate for the object
    // Returns: true if the new position is valid, false otherwise 
    bool setX(int _x) 
    {
        //checking for border violations
        if (_x > 78)
            return false;
        if (_x < 1)
            return false;
        else
            this->x = _x;

        return true;
    }

    // Sets a new y-coordinate for the object
    // Returns: true if the new position is valid, false otherwise
    bool setY(int _y) // Sets a new y
    {
        //checking for border violations
        if (_y > 23)
            return false;
        if (_y == 0)
            return false;
        else
            this->y = _y;

        return true;
    }

	void setSymbol(char s) {
		this->symbol = s;
	}

    // Draws the object at its current position
    void draw() {
        draw(symbol);
    }

    // Removes the object from its current position on the board
    // Replaces its symbol with the appropriate background character
    void remove(Board board);
};

#endif