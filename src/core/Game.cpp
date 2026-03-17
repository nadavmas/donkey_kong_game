#include "Game.h"
#include "GameSimple.h"
#include "GameSave.h"
#include "GameLoad.h"
#include "GameSilent.h"


// The Game class manages the entire game flow, including menus, gameplay, and state transitions.
// It initializes the game, tracks the current state, and handles player interactions with the game logic.
// Load screen files from the default directory
void Game::getAllBoardFileNames(std::vector<std::string>& vec_to_fill) const {
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".screen") {
			
			vec_to_fill.push_back(filenameStr);
		}
	}
}
void Game::getAllStepsFileNames(std::vector<std::string>& vec_to_fill) const {
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".steps") {
			
			vec_to_fill.push_back(filenameStr);
		}
	}
}
void Game::getAllResultFileNames(std::vector<std::string>& vec_to_fill) const  {
	namespace fs = std::filesystem;
	for (const auto& entry : fs::directory_iterator(fs::current_path())) {
		auto filename = entry.path().filename();
		auto filenameStr = filename.string();
		if (filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".result") {
			
			vec_to_fill.push_back(filenameStr);
		}
	}
}

 int Game::Play(int diff_lvl, Board& board, int score)
{
	return 0;
}
 int Game::Play(int diff_lvl, Board& board, int score, std::string& file_results, std::string& file_steps)
 {
	 return 0;
}


 void Game::run(int mode) {
	 int choice;
	 Menu menu;
	 menu.printMenu(); // Display the main menu
	 choice = menu.getChoice(); // Get the user's choice from the menu
	 if (choice == 1)
	 {
		 system("cls");
		 if (mode == 1)
		 {
			 GameSimple game;
			 game.run(mode);
		 }
		 else if (mode == 2)
		 {
			 GameSave game;
			 game.run(mode);
		 }
		 else if (mode == 3)
		 {
			 GameLoad game;
			 game.run(mode);
		 }
		 else if (mode == 4)
		 {
			 GameSilent game;
			 game.run(mode);
		 }
		
	 }
	 else {

		 // End the game and display a thank-you message
		 system("cls");
		 gotoxy(35, 12);
		 std::cout << "Thank you for playing Donkey-Kong!";

	 }

	 
 }






