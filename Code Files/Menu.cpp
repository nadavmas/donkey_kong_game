#include "Menu.h"

void Menu::printMenu() {
	for (int i = 0; i <= 79; i++) {
		gotoxy(i, 0); // Top border
		cout << '-';
		gotoxy(i, 24);// Bottom border
		cout << '-';
	}
	for (int i = 1; i <= 23; i++) {
		gotoxy(0, i); // Left border
		cout << '-';
		gotoxy(79, i); // Right border
		cout << '-';
	}
	gotoxy(27, 8);
	cout << "Welcome to Donkey Kong"; // Title
	gotoxy(27, 10);
	cout << "Press '1' to - Start Game";
	gotoxy(27, 12);
	cout << "Press '2' to - Exit";
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
		cout << " " << counter << ". " << num;
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

