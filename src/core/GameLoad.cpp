#include "GameLoad.h"

void GameLoad::run(int mode) {
	int score = 0;
	int choice = 1;
	int playVal, DL, boardChoice, counter, error;
	vector <string> names, stepsfiles, resultsfiles;
	std::string results_file, steps_file;
	Menu m;
	Board board;
	Steps steps;
	getAllResultFileNames(resultsfiles);
	getAllStepsFileNames(stepsfiles);
	getAllBoardFileNames(names);
	system("cls");
	
	if (choice == 1) {
		boardChoice = m.gameFlow(names);
		system("cls");
		counter = boardChoice;// Variables for tracking game state and difficulty level 
		results_file = resultsfiles[counter - 1];
		steps_file = stepsfiles[counter - 1];
		steps = Steps::loadSteps(steps_file);
		error = 0;
		error = board.load(names[boardChoice - 1]);
		if (error == BAD)
		{
			system("cls");
			m.errorInLoading();
			return;
		}
	
		system("cls"); // Clear the screen
		DL = steps.getDifficulty(); // Ask the user to select a difficulty level
		system("cls"); // Clear the screen again for a fresh start
	}
	else
		is_running = false;


	// Main game loop
	while (is_running == true)
	{

		if (choice == 1)
		{
			playVal = Play(DL, board, score, results_file, steps_file); // Start the game with the selected difficulty

		}
		else
			is_running = false; // Exit the game if the user chooses "Exit"
		board.resetLocations();
		score += 100;
		counter++;

		
		if (counter == 4)
			counter--;
		
		results_file = resultsfiles[counter - 1];
		steps_file = stepsfiles[counter - 1];
		system("cls"); // Clear the screen after the game session ends
		if ( counter < 4 && playVal == WINNING) {
			error = board.load(names[counter - 1]);
			choice = m.stageFinished();
			if (error == BAD)
			{
				system("cls");
				m.errorInLoading();
				is_running = false;
			}
			if (choice == 2)
				is_running = false; // Exit if the user chooses not to play again
		}
		// Handle the result of the game (Winning or Game Over)
		else if (playVal == WINNING)
		{
			choice = m.winningMessage(); // Display winning message and ask for next steps
			counter = 1;
			if (choice == 2)
				is_running = false; // Exit if the user chooses not to play again
		}
		else if (playVal == GAME_OVER) {

			choice = m.gameOverMessage(); // Display game-over message and ask for next steps
			if (choice == 2)
				is_running = false; // Exit if the user chooses not to play again
			else {
				counter--;
				score -= 100;
				error = board.load(names[counter - 1]);
				if (error == BAD)
				{
					system("cls");
					m.errorInLoading();
					is_running = false;
				}
			}
		}
		else
			is_running = false; // Exit for any other case

	}

	// End the game and display a thank-you message
	system("cls");
	gotoxy(35, 12);
	std::cout << "Thank you for playing Donkey-Kong!";

}

int GameLoad::Play(int diff_lvl, Board& board, int score, std::string& file_results, std::string& file_steps)
{
	system("cls");
	board.reset(); // Reset the board to its initial state
	board.print(); // Print the board to the screen
	Mario mario(board.getMario()); // Initialize Mario
	showLives(mario.getNumOfLives(), board.getL()); // Display Mario's remaining lives on the screen
	showScore(score, board.getL()); // Display the player's score on the screen
	DonkeyKong dk(board.getDK()); // Initialize Donkey Kong
	Princess pr(board.getPrincess()); // Initialize the Princess
	ShowConsoleCursor(false); // Hide the cursor for a cleaner game experience
	Barrel barrels[13] = { Barrel(board.getDK()), Barrel(board.getDK()), Barrel(board.getDK()),
	Barrel(board.getDK()), Barrel(board.getDK()), Barrel(board.getDK()),
	Barrel(board.getDK()), Barrel(board.getDK()), Barrel(board.getDK()),
	Barrel(board.getDK()), Barrel(board.getDK()), Barrel(board.getDK()),
	Barrel(board.getDK()) }; // Initialize an array of 13 barrels
	Menu m;
	std::vector<std::unique_ptr<Ghost>> ghosts;
	for (location num : board.getGhostsx())
		ghosts.push_back(std::make_unique<RegGhost>(num));
	for (location num : board.getGhostsX())
		ghosts.push_back(std::make_unique<SupGhost>(num));
	// Game state variables
	bool isExplosion = false, isLifeTaken = false;
	int  i = 1, MariofallCount = 0, barrelCount = 1, choice = 1;
	char input = '0'; // Player input, initialized to a default value
	int iterationCount = 0;
	Results results;
	Steps steps;

	results = Results::loadResults(file_results);
	steps = Steps::loadSteps(file_steps);
	auto curr_result = results.popResult();
	
	srand(steps.getRandomSeed()); // Seed the random number generator
	int it;
	if(!steps.getSteps().empty())
	  it = steps.getNextStep();
	std::mt19937 gen(steps.getRandomSeed());

	// Main game loop
	while (mario.getNumOfLives() > 0)
	{
		// Pause the game if ESC key is pressed
		if (input == 27)
		{
			system("cls"); // Clear the screen
			choice = m.pause(); // Show pause menu
			if (choice == 1)
			{
				system("cls");
				gotoxy(0, 0);
				board.print();
				mario.draw();
				dk.draw();
			}
			else
				return EXIT; // Exit the game if chosen from the pause menu
		}


		// Check for win condition (Mario reaching the Princess)
		if (isWinning(mario, pr))
		{
			curr_result.first++;
			if (curr_result.first == iterationCount && curr_result.second == Results::finished)
			{
				return WINNING; // Return winning state
			}
			results.reportResultError("Winning iteration mismatch", file_results, iterationCount, curr_result.first);
			break;
		}


		// Handle extra life collection
		if (isLifeTaken == false) {
			isExtraLife(board, mario, isLifeTaken);// Check if Mario have collected an extra life, if not checks if he got to the extra life
			showLives(mario.getNumOfLives(), board.getL()); // Update lives display
		}


		// Check and handle barrel explosions
		for (int i = 0; i < 13; i++) {
			if (barrels[i].getFallCount() >= 8) { // If the barrel has fallen enough
				if (board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == 'Q' || board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == '=' || board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == '>' || board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == '>' || barrels[i].getY() + 1 == 24) {
					barrels[i].explode(board); // Trigger barrel explosion
					if (isInExplosian(mario.getX(), mario.getY(), barrels[i].getX(), barrels[i].getY())) {
						if (curr_result.first == iterationCount && curr_result.second == Results::die) {
							mario.die(board); // Mario dies if he collides with Donkey Kong
							if(!results.getResults().empty())
								curr_result = results.popResult();
						}
						else {
							results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
							break;
						}
					}
					showLives(mario.getNumOfLives(), board.getL()); // Update lives display
					barrels[i].deactivate(board); // Deactivate the exploded barrel
				}
			}
		}


		// Spawn barrels periodically
		barrelCount--; // Decrease the countdown timer for barrel spawning
		if (barrelCount <= 0) { // Check if it's time to spawn a new barrel
			int k = 0;
			bool found = false; // Flag to track if an inactive barrel is found
			// Loop through the array of barrels to find an inactive barrel
			while (k < 13 && found == false) {
				if (barrels[k].getState() == false) { // Check if the barrel is inactive 
					barrels[k].activate(board); // Activate the barrel
					found = true; // Mark that a barrel has been activated
				}
				k++; // Move to the next barre
			}

			// Reset the barrel spawn timer based on the selected difficulty level
			setBarrelQnt(steps.getDifficulty(), barrelCount);/////////////8790
		}

		// Check collisions with Donkey Kong
		if (isHittingDK(board, mario)) {

			if (curr_result.first == iterationCount && curr_result.second == Results::die) {
				mario.die(board); // Mario dies if he collides with Donkey Kong
				showLives(mario.getNumOfLives(), board.getL()); // Update lives display
				dk.draw(); // Redraw Donkey Kong
				mario.setWalkingDirection(stay_);
				if (!results.getResults().empty())
					curr_result = results.popResult();
			}
			else {
				results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
				break;
			}

		}


		// Check collisions with barrels
		if (isHittingBarrel(board, mario, barrels)) {

			if (curr_result.first == iterationCount && curr_result.second == Results::die) {
				mario.die(board); // Mario dies if he collides with Donkey Kong
				showLives(mario.getNumOfLives(), board.getL()); // Update lives display
				mario.setWalkingDirection(stay_);
				if (!results.getResults().empty())
					curr_result = results.popResult();
			}
			else {
				results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
				break;
			}
		}
		if (isOnHammer(board, mario)) // Check if Mario is on a hammer
			mario.activateHammer(board); // Activate the hammer if Mario is on it

		if (isHittingGhost(board, mario, ghosts)) {

			if (curr_result.first == iterationCount && curr_result.second == Results::die) {
				mario.die(board); // Mario dies if he collides with Donkey Kong
				showLives(mario.getNumOfLives(), board.getL()); // Update lives display
				mario.setWalkingDirection(stay_);
				if (!results.getResults().empty())
					curr_result = results.popResult();
			}
			else {
				results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
				break;
			}
		}// Check collisions with the ghost


		// Handle Mario's falling logic
		if (fallingCheck(board, mario) && mario.isJumping() == false) {
			clearInputBuffer();  // Clear any queued input to prevent unwanted movement when landing
			mario.setFalling(true); // Set Mario to falling state
			MariofallCount++; // Increment the falling counter
			mario.setWalkingDirectionWhenFalling(board); // Adjust Mario's direction while falling
			mario.move(board); // Move Mario down
			Sleep(100);
		}
		else {
			mario.setFalling(false); // Clear falling state if on solid ground
			if (MariofallCount > 0) {
				if (mario.getWalkingDirection() == DOWN) // Adjust walking direction to stay after falling down
					mario.setWalkingDirection('s');
				else if (mario.getWalkingDirection() == DOWN_RIGHT) // Adjust walking direction to right after falling down-right
					mario.setWalkingDirection('d');
				else if (mario.getWalkingDirection() == DOWN_LEFT) // Adjust walking direction to left after falling down-left
					mario.setWalkingDirection('a');
			}
			// Reset fall count or kill Mario if falling for too long
			if (MariofallCount >= 5) {

				if (curr_result.first == iterationCount && curr_result.second == Results::die) {
					mario.die(board); // Mario dies if he collides with Donkey Kong
					showLives(mario.getNumOfLives(), board.getL()); // Update lives display
					mario.setWalkingDirection(stay_);
					MariofallCount = 0; // Reset the fall counter after Mario dies
					if (!results.getResults().empty())
						curr_result = results.popResult();
				}
				else {
					results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
					break;
				}
			}
			else
				MariofallCount = 0; // Reset the fall counter if Mario is not dead

			// If a jump has ended, reset Mario's state
			if (i == (mario.jump_height * 2) + 1)
			{
				mario.setJumping(false); // Set Mario's jumping state to false
				i = 1; // Reset the jump iteration counter
				mario.jump_height = default_jump_height;  // Reset Mario's jump height to default
				clearInputBuffer();  // Clear any queued keyboard input to avoid interference
				if (fallingCheck(board, mario))  // Check if Mario is falling after the jump
					mario.setFalling(true); // Set Mario to falling state if he is falling
			}

			while (iterationCount < it && mario.isJumping() == false && mario.getNumOfLives() > 0)
			{

				// Check for win condition (Mario reaching the Princess)
				if (isWinning(mario, pr))
				{
					curr_result.first++;
					if (curr_result.first == iterationCount && curr_result.second == Results::finished)
					{
						return WINNING; // Return winning state
					}
					results.reportResultError("Winning iteration mismatch", file_results, iterationCount, curr_result.first);
					break;
				}

				// Handle extra life collection
				if (isLifeTaken == false) {
					isExtraLife(board, mario, isLifeTaken); // Check if Mario have collected an extra life, if not checks if he got to the extra life
					showLives(mario.getNumOfLives(), board.getL()); // Update lives display
				}


				// Check and handle barrel explosions
				for (int i = 0; i < 13; i++) {
					if (barrels[i].getFallCount() >= 8) { // If the barrel has fallen enough
						if (board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == 'Q' || board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == '=' || board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == '>' || board.getChar(barrels[i].getX(), barrels[i].getY() + 1) == '>' || barrels[i].getY() + 1 == 24) {
							barrels[i].explode(board); // Trigger barrel explosion
							if (isInExplosian(mario.getX(), mario.getY(), barrels[i].getX(), barrels[i].getY())) {

								if (curr_result.first == iterationCount && curr_result.second == Results::die) {
									mario.die(board); // Mario dies if he collides with Donkey Kong
									if (!results.getResults().empty())
										curr_result = results.popResult();
								}
								else {
									results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
									break;
								}
							}
							showLives(mario.getNumOfLives(), board.getL()); // Update lives display
							barrels[i].deactivate(board); // Deactivate the exploded barrel
						}
					}
				}

				// Spawn barrels periodically
				barrelCount--; // Decrease the countdown timer for barrel spawning
				if (barrelCount <= 0) { // Check if it's time to spawn a new barrel
					int k = 0;
					bool found = false; // Flag to track if an inactive barrel is found
					// Loop through the array of barrels to find an inactive barrel
					while (k < 13 && found == false) { // Check if the barrel is inactive
						if (barrels[k].getState() == false) { // Activate the barrel
							barrels[k].activate(board); // Mark that a barrel has been activated
							found = true;
						}
						k++; // Move to the next barrel
					}

					// Reset the barrel spawn timer based on the selected difficulty level
					setBarrelQnt(steps.getDifficulty(), barrelCount);
				}


				// Check collisions with Donkey Kong
				if (isHittingDK(board, mario)) {

					if (curr_result.first == iterationCount && curr_result.second == Results::die) {
						mario.die(board); // Mario dies if he collides with Donkey Kong
						showLives(mario.getNumOfLives(), board.getL()); // Update lives display
						dk.draw(); // Redraw Donkey Kong
						mario.setWalkingDirection(stay_);
						if (!results.getResults().empty())
							curr_result = results.popResult();
					}
					else {
						results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
						break;
					}
				}


				// Check collisions with barrels
				if (isHittingBarrel(board, mario, barrels)) {

					if (curr_result.first == iterationCount && curr_result.second == Results::die) {
						mario.die(board); // Mario dies if he collides with Donkey Kong
						showLives(mario.getNumOfLives(), board.getL()); // Update lives display
						mario.setWalkingDirection(stay_);
						if (!results.getResults().empty())
							curr_result = results.popResult();
					}
					else {
						results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
						break;
					}
				}
				if (isOnHammer(board, mario)) // Check if Mario is on a hammer
					mario.activateHammer(board); // Activate the hammer if Mario is on it

				if (isHittingGhost(board, mario, ghosts)) {

					if (curr_result.first == iterationCount && curr_result.second == Results::die) {
						mario.die(board); // Mario dies if he collides with Donkey Kong
						showLives(mario.getNumOfLives(), board.getL()); // Update lives display
						mario.setWalkingDirection(stay_);
						if (!results.getResults().empty())
							curr_result = results.popResult();
					}
					else {
						results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
						break;
					}
				}// Check collisions with the ghost




				// Handle Mario's falling logic inside 
				if (fallingCheck(board, mario) && mario.isJumping() == false) {
					clearInputBuffer();   // Clear any queued input to prevent unwanted movement when landing
					mario.setFalling(true); // Set Mario to falling state	
					MariofallCount++; // Increment the falling counter
					mario.setWalkingDirectionWhenFalling(board); // Adjust Mario's direction while falling
					mario.move(board); // Move Mario down
					Sleep(100);

				}
				else {
					mario.setFalling(false); // Clear falling state if on solid ground
					if (MariofallCount > 0) {
						if (mario.getWalkingDirection() == DOWN) // Adjust walking direction to stay after falling down
							mario.setWalkingDirection('s');
						else if (mario.getWalkingDirection() == DOWN_RIGHT) // Adjust walking direction to right after falling down-right
							mario.setWalkingDirection('d');
						else if (mario.getWalkingDirection() == DOWN_LEFT) // Adjust walking direction to left after falling down-left
							mario.setWalkingDirection('a');
					}
					// Reset fall count or kill Mario if falling for too long
					if (MariofallCount >= 5) {

						if (curr_result.first == iterationCount && curr_result.second == Results::die) {
							mario.die(board); // Mario dies if he collides with Donkey Kong
							showLives(mario.getNumOfLives(), board.getL()); // Update lives display
							mario.setWalkingDirection(stay_);
							MariofallCount = 0; // Reset the fall counter after Mario dies
							if (!results.getResults().empty())
								curr_result = results.popResult();
						}
						else {
							results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
							break;
						}

					}
					else
						MariofallCount = 0; // Reset the fall counter if Mario is not dead


					// Handle wall collisions
					if (isHittingWall(board, mario))
					{
						input = mario.getWalkingDirection(); // Adjust Mario's movement
					}


					if (mario.getWalkingDirection() == UP || mario.getWalkingDirection() == DOWN)
					{
						if (isOnLadder(board, mario, input) == false) // Stop climbing if Mario is no longer on a ladder
							mario.setWalkingDirection(stay_);
					}

					mario.move(board); // Move Mario after all checks
					setSleep(steps.getDifficulty()); // Control game speed based on difficulty
				}

				// If Mario starts falling again, set falling state
				if (fallingCheck(board, mario))
					mario.setFalling(true);


				// Check barrel collisions again after movement
				if (isHittingBarrel(board, mario, barrels)) {

					if (curr_result.first == iterationCount && curr_result.second == Results::die) {
						mario.die(board); // Mario dies if he collides with Donkey Kong
						showLives(mario.getNumOfLives(), board.getL()); // Update lives display
						mario.setWalkingDirection(stay_);
						if (!results.getResults().empty())
							curr_result = results.popResult();
					}
					else {
						results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
						break;
					}
				}
				ghostsDirections(ghosts);
				moveBarrel(board, barrels); // Move all barrels on the board////7/67/6/76/
				moveGhost(board, ghosts, gen); // Move the ghost
				iterationCount++;

			}


			if (mario.getNumOfLives() == 0)
				break;
			// Handle player input
			if (mario.isJumping() == false) {
				if (!steps.getSteps().empty())
					input = steps.popStep(); // Capture player input
				if (!steps.getSteps().empty())
					it = steps.getNextStep();
					
						
			}
			if ((input == 'P' || input == 'p') && mario.isHoldingHammerF()) {
				if (curr_result.first == iterationCount && curr_result.second == Results::useHammer)
				{

					mario.useHammer(board, ghosts, barrels); // Activate the hammer if Mario is on it
					if (!results.getResults().empty())
						curr_result = results.popResult();
				}
				else {
					results.reportResultError("Hammer iteration mismatch", file_results, iterationCount, curr_result.first);
					break;
				}
				
			}
			// Handle jumping
			else if (input == 'w' || input == 'W')
			{
				if (!isOnLadder(board, mario, input)) {
					if (i == 1) {

						int k = 1;
						while (k <= default_jump_height) {
							// Calculate Mario's jump height dynamically

							// Case 1: Mario is stationary (STAY) and detects an obstacle directly above
							if (mario.getWalkingDirection() == STAY)
								if (board.getChar(mario.getX(), mario.getY() - k) == '=' || board.getChar(mario.getX(), mario.getY() - k) == 'Q') {
									mario.jump_height = k - 1; // Set the jump height just below the obstacle
									break; // Exit the loop since the jump height has been determined
								}

							// Case 2: Mario is moving (left or right) and detects an obstacle above in the direction of movement
							if (board.getChar(mario.getX() - (k * pow(-1, mario.getWalkingDirection())), mario.getY() - k) == '=' || board.getChar(mario.getX(), mario.getY() - k) == 'Q') {
								mario.jump_height = k - 1; // Set the jump height based on the obstacle detected
								break; // Exit the loop
							}
							k++; // Increment the step height to check higher obstacles

						}
					}
					if (mario.isFalling() == false)
					{
						if (mario.jump_height > 0) {
							mario.setJumping(true); // Set jumping state
							mario.jump(board, i); // Execute jump
							Sleep(100);
							i++; // Increment the jump iteration
						}
						else
							mario.jump_height = default_jump_height;
					}
				}
				else
					mario.setWalkingDirection(input); // Handle ladder climbing
			}


			// Handle climbing down
			else if (input == 's' || input == 'S')
			{
				if (mario.isFalling() == false) { // Check if Mario is not falling
					mario.setWalkingDirection(input);
					if (!isOnLadder(board, mario, input))
						mario.setWalkingDirection(stay_);
				}
			}
			else {
				if (!isOnLadder(board, mario, input))
					mario.setWalkingDirection(input);
			}


		}

		// Final check for collisions with barrels after movement
		if (isHittingBarrel(board, mario, barrels)) {

			if (curr_result.first == iterationCount && curr_result.second == Results::die) {
				mario.die(board); // Mario dies if he collides with Donkey Kong
				showLives(mario.getNumOfLives(), board.getL()); // Update lives display
				mario.setWalkingDirection(stay_);
				if (!results.getResults().empty())
					curr_result = results.popResult();
			}
			else {
				results.reportResultError("Death iteration mismatch", file_results, iterationCount, curr_result.first);
				break;
			}
		}
		ghostsDirections(ghosts);
		moveBarrel(board, barrels); // Move barrels again for the current frame
		moveGhost(board, ghosts, gen); // Move the ghost
		iterationCount++;
		

	}
	
	return GAME_OVER;// Return game-over state if Mario runs out of lives

}