#include "GameObject.h"


// Removes the object from its current position on the board
// Replaces its symbol with the appropriate background character
void GameObject::remove(Board board) {
    char above = board.getChar(x, y - 1);
    char under = board.getChar(x, y + 1);
    char left = board.getChar(x - 1, y);
    char right = board.getChar(x + 1, y);
    //Checking all cases in which an object is moving past a ladder
    if (under == 'H' && above == '=')
        draw('H');
    else if (above == 'H' && under == 'H')
        draw('H');
    else if (above == 'H' && under == 'Q')
        draw('H');
    else if (above == 'H' && this->getY() + 1 == 24)
        draw('H');
    else if (above == 'H' && under == '=')
        draw('H');
    else if (above == 'H' && under == '<')
        draw('H');
    else if (above == 'H' && under == '>')
        draw('H');
    else if (x == board.getHammer().x && y == board.getHammer().y && !board.getHammerTaken())
        draw('p');
    else if (x == board.getPrincess().x && y == board.getPrincess().y)
        draw('$');
    else
        draw(' '); // Default to clearing the character
    
}
