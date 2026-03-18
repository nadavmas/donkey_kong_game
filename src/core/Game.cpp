#include "Game.h"
#include "GameSimple.h"
#include "GameSave.h"
#include "GameLoad.h"
#include "GameSilent.h"

#include <chrono>
#include <fstream>
#include <string>

static std::string jsonEscape(const std::string& in) {
	std::string out;
	out.reserve(in.size());
	for (char c : in) {
		if (c == '\\') out += "\\\\";
		else if (c == '"') out += "\\\"";
		else if (c == '\n') out += "\\n";
		else if (c == '\r') out += "\\r";
		else out += c;
	}
	return out;
}


// The Game class manages the entire game flow, including menus, gameplay, and state transitions.
// It initializes the game, tracks the current state, and handles player interactions with the game logic.
// Load screen files from the default directory
void Game::getAllBoardFileNames(std::vector<std::string>& vec_to_fill) const {
	namespace fs = std::filesystem;
	auto scanDir = [&](const fs::path& dir) {
		int found = 0;
		if (!fs::exists(dir) || !fs::is_directory(dir)) {
			return 0;
		}
		for (const auto& entry : fs::directory_iterator(dir)) {
			auto filename = entry.path().filename();
			auto filenameStr = filename.string();
			if (filenameStr.size() >= 5 && filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".screen") {
				vec_to_fill.push_back((dir / filename).string());
				found++;
			}
		}
		return found;
	};

	// 1) Working directory (original behavior)
	const fs::path cwd = fs::current_path();
	if (scanDir(cwd) > 0) return;

	// 2) Common repo layout fallback: ../assets and ../../assets from project folder
	// This keeps the game runnable even if Visual Studio working directory is the project dir.
	const fs::path assets1 = cwd / "assets";
	if (scanDir(assets1) > 0) return;
	const fs::path assets2 = cwd.parent_path() / "assets";
	if (scanDir(assets2) > 0) return;
	const fs::path assets3 = cwd.parent_path().parent_path() / "assets";
	(void)scanDir(assets3);
}
void Game::getAllStepsFileNames(std::vector<std::string>& vec_to_fill) const {
	namespace fs = std::filesystem;
	const fs::path cwd = fs::current_path();
	auto scanDir = [&](const fs::path& dir) {
		int found = 0;
		if (!fs::exists(dir) || !fs::is_directory(dir)) return 0;
		for (const auto& entry : fs::directory_iterator(dir)) {
			auto filename = entry.path().filename();
			auto filenameStr = filename.string();
			if (filenameStr.size() >= 5 && filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".steps") {
				vec_to_fill.push_back((dir / filename).string());
				found++;
			}
		}
		return found;
	};

	// 1) Working directory
	int found = scanDir(cwd);
	// 2) Common repo layout fallback: assets adjacent to project
	if (found == 0) {
		const fs::path assets1 = cwd / "assets";
		found = scanDir(assets1);
	}
	if (found == 0) {
		const fs::path assets2 = cwd.parent_path() / "assets";
		found = scanDir(assets2);
	}
	if (found == 0) {
		const fs::path assets3 = cwd.parent_path().parent_path() / "assets";
		(void)scanDir(assets3);
	}

	// #region debug_save_discovery_steps
	{
		const auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
		std::ofstream f("C:\\Projects\\donkey_kong_game\\debug-15106d.log", std::ios::app);
		if (f.is_open()) {
			const size_t found = vec_to_fill.size();
			const std::string cwdStr = jsonEscape(cwd.string());
			const std::string sample = (found > 0) ? jsonEscape(vec_to_fill.front()) : "";
			f << "{\"sessionId\":\"15106d\",\"runId\":\"pre-fix\",\"hypothesisId\":\"H_save_discovery_workdir\",\"location\":\"Game.cpp:getAllStepsFileNames\",\"message\":\"stepsDiscovery\",\"data\":{\"cwd\":\""
			  << cwdStr << "\",\"foundCount\":" << found << ",\"sample0\":\"" << sample << "\"},\"timestamp\":" << ts << "}\n";
		}
	}
	// #endregion
}
void Game::getAllResultFileNames(std::vector<std::string>& vec_to_fill) const  {
	namespace fs = std::filesystem;
	const fs::path cwd = fs::current_path();
	auto scanDir = [&](const fs::path& dir) {
		int found = 0;
		if (!fs::exists(dir) || !fs::is_directory(dir)) return 0;
		for (const auto& entry : fs::directory_iterator(dir)) {
			auto filename = entry.path().filename();
			auto filenameStr = filename.string();
			if (filenameStr.size() >= 5 && filenameStr.substr(0, 5) == "dkong" && filename.extension() == ".result") {
				vec_to_fill.push_back((dir / filename).string());
				found++;
			}
		}
		return found;
	};

	// 1) Working directory
	int found = scanDir(cwd);
	// 2) Common repo layout fallback
	if (found == 0) {
		const fs::path assets1 = cwd / "assets";
		found = scanDir(assets1);
	}
	if (found == 0) {
		const fs::path assets2 = cwd.parent_path() / "assets";
		found = scanDir(assets2);
	}
	if (found == 0) {
		const fs::path assets3 = cwd.parent_path().parent_path() / "assets";
		(void)scanDir(assets3);
	}

	// #region debug_save_discovery_results
	{
		const auto ts = std::chrono::duration_cast<std::chrono::milliseconds>(
			std::chrono::system_clock::now().time_since_epoch()).count();
		std::ofstream f("C:\\Projects\\donkey_kong_game\\debug-15106d.log", std::ios::app);
		if (f.is_open()) {
			const size_t found = vec_to_fill.size();
			const std::string cwdStr = jsonEscape(cwd.string());
			const std::string sample = (found > 0) ? jsonEscape(vec_to_fill.front()) : "";
			f << "{\"sessionId\":\"15106d\",\"runId\":\"pre-fix\",\"hypothesisId\":\"H_save_discovery_workdir\",\"location\":\"Game.cpp:getAllResultFileNames\",\"message\":\"resultsDiscovery\",\"data\":{\"cwd\":\""
			  << cwdStr << "\",\"foundCount\":" << found << ",\"sample0\":\"" << sample << "\"},\"timestamp\":" << ts << "}\n";
		}
	}
	// #endregion
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






