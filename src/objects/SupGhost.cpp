#include "SupGhost.h"

int SupGhost::chooseState(int curr, int alter, Board& board, std::mt19937 gen) {
    // Create a random number generator

    // Create a distribution for our probability
    std::uniform_real_distribution<> dis(0, 1);
    if ((isUnderLadder(board) == false && isAboveladder(board) == false) || (isUnderLadder(board) == true && movingDirection == DOWN) || (isAboveladder(board) == true && movingDirection == UP)) {

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
    else {
        double randomNumber = dis(gen);
        double randomNumber1 = dis(gen);
        if (randomNumber1 < 0.25 && isUnderLadder(board))
            return UP;
        else if (randomNumber1 >= 0.75 && isAboveladder(board))
            return DOWN;
        else {
            if (randomNumber < 0.95) {
                return curr;
            }
            else {
                // 5% chance to switch to the alternate state
                return alter;
            }
        }


    }
}
void SupGhost::move(Board& board, std::mt19937 gen) {
    if (isOnLadder(board)) {
        if (movingDirection == UP) {
            if (board.getChar(this->getX(), this->getY() - 1) != 'Q') {
                if (board.getChar(this->getX(), this->getY() - 1) == '=')
                {
                    gotoxy(getX(), getY());
                    remove(board);
                    setY(getY() - 2); // Move the ghost up
                    draw();
                }
                else {
                    gotoxy(getX(), getY());
                    remove(board);
                    setY(getY() - 1); // Move the ghost up
                    draw();
                }
            }
            else {
                setMovingDirection(DOWN); // If blocked, switch direction to the right
                if (board.getChar(this->getX(), this->getY() + 1) == '=') {
                    gotoxy(getX(), getY());
                    remove(board);
                    setY(getY() + 2); // Move the ghost down
                    draw();
                }
                else {
                    gotoxy(getX(), getY());
                    remove(board);
                    setY(getY() + 1); // Move the ghost down
                    draw();
                }

            }
        }
        else if (movingDirection == DOWN) {
            if (board.getChar(this->getX(), this->getY() + 1) != 'Q') {
                if (board.getChar(this->getX(), this->getY() + 1) == '=') {
                    gotoxy(getX(), getY());
                    remove(board);
                    setY(getY() + 2); // Move the ghost down
                    draw();
                }
                else {
                    gotoxy(getX(), getY());
                    remove(board);
                    setY(getY() + 1); // Move the ghost down
                    draw();
                }
            }
            else {
                setMovingDirection(UP); // If blocked, switch direction to the right
                gotoxy(getX(), getY());
                remove(board);
                setY(getY() - 1); // Move the ghost up
                draw();
            }

        }
    }
    else {
        if (movingDirection == UP || movingDirection == DOWN)
            movingDirection = chooseState(LEFT, RIGHT, board, gen);
        int otherDirection = (movingDirection == LEFT) ? RIGHT : LEFT;
        movingDirection = chooseState(movingDirection, otherDirection, board, gen);
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
        }     if (movingDirection == UP) {
            if (board.getChar(this->getX(), this->getY() - 1) != 'Q') {
                if (board.getChar(this->getX(), this->getY() - 1) == '=')
                {
                    gotoxy(getX(), getY());
                    remove(board);
                    setY(getY() - 2); // Move the ghost up
                    draw();
                }
                else {
                    gotoxy(getX(), getY());
                    remove(board);
                    setY(getY() - 1); // Move the ghost up
                    draw();
                }
            }
            else {
                setMovingDirection(DOWN); // If blocked, switch direction to the right
                gotoxy(getX(), getY());
                remove(board);
                setY(getY() + 1); // Move the ghost down
                draw();
            }
        }
        else if (movingDirection == DOWN) {
            if (board.getChar(this->getX(), this->getY() + 1) != 'Q') {
                if (board.getChar(this->getX(), this->getY() + 1) == '=') {
                    gotoxy(getX(), getY());
                    remove(board);
                    setY(getY() + 2); // Move the ghost down
                    draw();
                }
                else {
                    gotoxy(getX(), getY());
                    remove(board);
                    setY(getY() + 1); // Move the ghost down
                    draw();
                }
            }
            else {
                setMovingDirection(UP); // If blocked, switch direction to the right
                gotoxy(getX(), getY());
                remove(board);
                setY(getY() - 1); // Move the ghost up
                draw();
            }

        }
    }


}