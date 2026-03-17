#include "Helpers.h"

#include <iostream>

using namespace std;

// Moves the console cursor to the specified position (x, y)
void gotoxy(int x, int y) {
    std::cout.flush(); // Ensure all output is displayed before moving the cursor
    COORD coord;  // Create a COORD structure for the new position
    coord.X = x;  
    coord.Y = y; 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Move the cursor 
}

// Toggles the visibility of the console cursor
void ShowConsoleCursor(bool showFlag) {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO     cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo); // Get the current cursor info
    cursorInfo.bVisible = showFlag; // set the cursor visibility
    SetConsoleCursorInfo(out, &cursorInfo); // Apply the new visibility setting
}

// Displays the player's remaining lives visually on the console
void showLives(int num_lives,location L)
{
    gotoxy(L.x+6, L.y+1); // Move to the position where lives are displayed
    if (num_lives == 4)
        cout << " <3 <3 <3 <3"; // Display 4 lives
    else if (num_lives == 3)
        cout << " <3 <3 <3   "; // Display 3 lives
    else if (num_lives == 2)
        cout << " <3 <3      "; // Display 2 lives
    else if (num_lives == 1)
        cout << " <3         "; // Display 1 life
    else
        cout << "            "; // Display nothing if no lives are left

}

// Displays the player's score visually on the console
void showScore(int score, location L)
{
	gotoxy(L.x +26, L.y + 1); // Move to the position where the score is displayed
	cout << " " << score; // Display the current score
}

// Sets the sleep duration between game cycles based on the difficulty level
void setSleep(int diff_lvl) {
    if (diff_lvl == 1) // Longer delay for easy level
        Sleep(170);
    else if (diff_lvl == 2) // Medium delay for intermediate level
        Sleep(100);
    else if (diff_lvl == 3)// Short delay for advanced level
		Sleep(70);
    else
        Sleep(0);
}

// Sets the number of iterations between each barrel spawn based on the difficulty level
void setBarrelQnt(int diff_lvl,int& barrelCount) {
    if (diff_lvl == 1) // Longer interval between barrel spawns for easier level
        barrelCount = 40;
    else if (diff_lvl == 2) // Medium interval for intermediate level
        barrelCount = 30; 
    else // Shorter interval for advanced level
        barrelCount = 20;

}

// Clears any remaining input from the console input buffer
void clearInputBuffer() {
    while (_kbhit()) { // Check if there is input in the buffer
        _getch();       // Read and discard the input
    }
}

