#include <iostream> 
#include "Game.h"
#include "GameSimple.h"
#include "GameSave.h"
#include "GameLoad.h"
#include "Menu.h"

int main(int argc, char** argv)
{
	int mode;
	if (argc > 1 && std::string(argv[1]) == "-save")
		mode = 2;
	else if (argc > 2 && std::string(argv[1]) == "-load" && std::string(argv[2]) == "-silent")
		mode = 4;
	else if (argc > 1 && std::string(argv[1]) == "-load")
		mode = 3;
	else
		mode = 1;

	Game game;
	game.run(mode);

	gotoxy(0, 27);
	return 0; // Return 0 indicates successful execution
}