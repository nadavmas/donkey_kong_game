#ifndef DonkeyKong_h
#define DonkeyKong_h

#include "GameObject.h"
#include "Barrel.h"

// The DonkeyKong class represents the Donkey Kong character in the game.
// Donkey Kong is responsible for throwing barrels that create challenges for the player.

class DonkeyKong : public GameObject {
	int itBetweenBarrels; // Tracks the interval between throwing barrels
	
public:
	// Constructor initializes Donkey Kong's symbol and position
	// Sets the interval for throwing barrels to 8
	DonkeyKong(location l) : GameObject('&',l), itBetweenBarrels(8) {
		draw();
	}


	// Activates and throws the barrel
	void throwBarrel(Barrel& barrel, const Board& board) {
		barrel.draw(); // Draws the barrel
		barrel.activate(board); // Activates the barrel
	}
};
#endif