#include "RegGhost.h"

// Function to randomly decide the direction the ghost should move
int RegGhost::chooseState(int curr, int alter, Board& board, std::mt19937 gen) {
    // Create a distribution for our probability
    std::uniform_real_distribution<> dis(0, 1);

    // Generate a random number between 0 and 1
    double randomNumber = dis(gen);

    // 95% chance to keep the current state
    if (randomNumber < 0.95) {
        return curr;
    }
    else {
        // 5% chance to switch to the alternate state
        return alter;
    }
}

// Moves the ghost based on its direction
void RegGhost::move(Board& board, std::mt19937 gen) {
    int otherDirection = (movingDirection == LEFT) ? RIGHT : LEFT;
    movingDirection = chooseState(movingDirection, otherDirection, board, gen); // Randomly choose the direction

    if (movingDirection == LEFT) {
        // Move left if there's no obstacle
        if (board.getChar(this->getX() - 1, this->getY() + 1) != ' ' && board.getChar(this->getX() - 1, this->getY()) != 'Q') {
            gotoxy(getX(), getY());
            remove(board);
            setX(getX() - 1); // Move the ghost left
            draw();
        }
        else {
            setMovingDirection(RIGHT); // If blocked, switch direction to the right
            gotoxy(getX(), getY());
            remove(board);
            setX(getX() + 1); // Move the ghost right
            draw();
        }
    }
    else if (movingDirection == RIGHT) {
        // Move right if there's no obstacle
        if (board.getChar(this->getX() + 1, this->getY() + 1) != ' ' && board.getChar(this->getX() + 1, this->getY()) != 'Q') {
            gotoxy(getX(), getY());
            remove(board);
            setX(getX() + 1); // Move the ghost right
            draw();
        }
        else {
            setMovingDirection(LEFT); // If blocked, switch direction to the left
            gotoxy(getX(), getY());
            remove(board);
            setX(getX() - 1); // Move the ghost left
            draw();
        }
    }
}
