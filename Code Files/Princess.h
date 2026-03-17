#ifndef Princess_h
#define Princess_h

// The Princess class represents the goal object in the game.
// The player wins by reaching the Princess's position.

#include "GameObject.h"

class Princess : public GameObject{
public:
	// Constructor initializes the Princess with the symbol '$' and sets its position
	Princess(location l) : GameObject('$',l) {
		draw(); // Draw the Princess on the board at its initial position
	}
	

};





#endif // !Princess_h
