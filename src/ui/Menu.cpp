#include "Menu.h"

#include <cstdlib>
#include <cstring>

namespace {

void printCenteredRow(int row, const char* text) {
	const int innerLeft = 1;
	const int innerWidth = 78;
	const int len = static_cast<int>(strlen(text));
	int x = innerLeft + (innerWidth - len) / 2;
	if (x < innerLeft) {
		x = innerLeft;
	}
	gotoxy(x, row);
	cout << text;
}

void drawOuterFrame() {
	gotoxy(0, 0);
	cout << '+';
	for (int x = 1; x <= 78; ++x) {
		gotoxy(x, 0);
		cout << '-';
	}
	gotoxy(79, 0);
	cout << '+';

	gotoxy(0, 24);
	cout << '+';
	for (int x = 1; x <= 78; ++x) {
		gotoxy(x, 24);
		cout << '-';
	}
	gotoxy(79, 24);
	cout << '+';

	for (int y = 1; y <= 23; ++y) {
		gotoxy(0, y);
		cout << '|';
		gotoxy(79, y);
		cout << '|';
	}
}

} // namespace

void Menu::printMenu() {
	system("cls");
	drawOuterFrame();

	// ASCII title block (plain ASCII; each line centered in the inner area)
	static const char* const kBanner[] = {
		" ____   ___ _  _  _  ___  _  _  ____  _  _ ",
		"|  _ \\ / _ \\| \\| |/ |/ _ \\| \\| |/ ___|| \\| |",
		"| | | | | | | .' | ' | | | | .' | |  _ | .' |",
		"| |_| | |_| | |\\ | .' | |_| | |\\ | |_| | |\\ |",
		"|____/ \\___/|_| \\_|_|\\_|\\___/|_| \\|\\____|_| \\|",
	};
	const int bannerLines = static_cast<int>(sizeof(kBanner) / sizeof(kBanner[0]));
	const int bannerStartRow = 2;
	for (int i = 0; i < bannerLines; ++i) {
		printCenteredRow(bannerStartRow + i, kBanner[i]);
	}

	printCenteredRow(8, "~~~  CONSOLE EDITION  ~~~");
	printCenteredRow(9, "=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-");

	printCenteredRow(11, "[1]  Start Game");
	printCenteredRow(12, "[2]  Exit");
	printCenteredRow(14, "------------------------------------------");
}

int Menu::getChoice() {
	int choice;
	gotoxy(27, 16);
	cout << "Please enter your choice: ";
	cin >> choice;

	// Ensure the choice is valid
	while (choice != 1 && choice != 2)
	{
		gotoxy(53, 16); // Reset cursor to the input position
		cout << "  ";   // Clear the invalid input
		gotoxy(53, 16); // Reposition cursor
		cin >> choice;  // Read the new input
	}
	return choice;
}

int Menu::winningMessage() {
	int choice;
	gotoxy(27, 10);
	cout << "you have won! would you like to play again?";
	gotoxy(27, 12);
	cout << "Press '1' to - Start New Game";
	gotoxy(27, 14);
	cout << "Press '2' to - Exit";
	choice = getChoice();

	// Ensure the choice is valid, same way as in getChoice() function
	while (choice != 1 && choice != 2)
	{
		gotoxy(53, 16);
		cout << "  ";
		gotoxy(53, 16);
		cin >> choice;

	}
	return choice;
}

int Menu::stageFinished() {
	int choice;
	gotoxy(27, 10);
	cout << "you have finished this stage, would you like to continue?";
	gotoxy(27, 12);
	cout << "Press '1' to - Start New Game";
	gotoxy(27, 14);
	cout << "Press '2' to - Exit";
	choice = getChoice();

	// Ensure the choice is valid, same way as in getChoice() function
	while (choice != 1 && choice != 2)
	{
		gotoxy(53, 16);
		cout << "  ";
		gotoxy(53, 16);
		cin >> choice;

	}
	return choice;
}

int Menu::pause() {
	int choice;
	gotoxy(27, 10);
	cout << "The game is paused. would you like to continue playing?";
	gotoxy(27, 12);
	cout << "Press '1' to - Continue";
	gotoxy(27, 14);
	cout << "Press '2' to - Exit";
	choice = getChoice();
	// Ensure the choice is valid, same way as in getChoice() function
	while (choice != 1 && choice != 2)
	{
		gotoxy(53, 16);
		cout << "  ";
		gotoxy(53, 16);
		cin >> choice;

	}
	return choice;

}

int Menu::gameOverMessage() {
	int choice;
	gotoxy(27, 10);
	cout << "You died! would you like to play again?";
	gotoxy(27, 12);
	cout << "Press '1' to - Start New Game";
	gotoxy(27, 14);
	cout << "Press '2' to - Exit";
	choice = getChoice();

	// Ensure the choice is valid, same way as in getChoice() function
	while (choice != 1 && choice != 2)
	{
		gotoxy(53, 16);
		cout << "  ";
		gotoxy(53, 16);
		cin >> choice;
	}
	return choice;
}

int Menu::difficultyLevel() {
	int choice;
	gotoxy(27, 8);
	cout << "Please choose difficulty level: ";
	gotoxy(27, 10);
	cout << "Press '1' for - Beginner";
	gotoxy(27, 12);
	cout << "Press '2' for - Intermediate";
	gotoxy(27, 14);
	cout << "Press '3' for - Advanced";
	gotoxy(27, 16);
	cout << "Please enter your choice: ";
	cin >> choice;

	// Ensure the choice is valid, same way as in getChoice() function (but with 3 options)
	while (choice != 1 && choice != 2 && choice != 3)
	{
		gotoxy(53, 16);
		cout << "  ";
		gotoxy(53, 16);
		cin >> choice;
	}
	return choice;
}

void Menu::errorInLoading() {
	gotoxy(27, 10);
	cout << "Error in loading the board. Please check the file and try again.";
	gotoxy(27, 12);
	cout << "Press any key to exit.";
	_getch();
}

// This function displays the level selection menu and gets the user's choice for a level
int Menu::gameFlow(const vector<string>& files) {
	int choice;
	int counter = 1;

	// Display the available stages
	gotoxy(27, 8);
	cout << "Please choose a level: ";

	// Loop through the vector to display each stage
	for (const string& num : files) {
		gotoxy(27, 8 + counter);
		cout << " " << counter << ". Level " << counter;
		counter++;
	}

	// Ask the player to enter their choice
	while (true) {
		gotoxy(27, 8 + counter + 1); // Move cursor below the options
		cout << "Enter your choice (1-" << files.size() << "): ";

		// Read the input and validate
		if (cin >> choice && choice >= 1 && choice <= files.size()) {
			break; // Valid input, exit loop
		}
		else {
			// If invalid input (non-numeric or out of range)
			cin.clear(); // Clear any error flags from cin
			cin.ignore(10000, '\n'); // Ignore the invalid input in the buffer
			gotoxy(27, 8 + counter + 2); // Move the cursor below the error message
			cout << "Invalid input. Please enter a number between 1 and " << files.size() << ".";
		}
	}

	return choice; // Return the valid choice
}

