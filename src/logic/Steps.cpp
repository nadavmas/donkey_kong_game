#include <fstream>
#include <string>
#include <iostream>


#include "Steps.h"

// This function loads the steps from a specified file.
Steps Steps::loadSteps(const std::string& filename) {
	Steps steps; // Create a Steps object to store the data

	// Open the steps file for reading
	std::ifstream steps_file(filename);
	if (!steps_file.is_open()) // Check for opening error
	{
		std::cout << "Error: Could not open steps file " << filename << std::endl;
		return steps;  // Return an empty Steps object
	}

	// Read the difficulty level and random seed from the file
	steps_file >> steps.difficulty;
	steps_file >> steps.randomSeed;

	size_t size; // Variable to store the number of steps
	steps_file >> size;

	// Read each step from the file and add it to the Steps object
	while (!steps_file.eof() && size-- != 0) {
		size_t iteration;
		char step;
		steps_file >> iteration >> step;
		steps.addStep(iteration, step);
	}
	steps_file.close();// Close the file after reading
	return steps; // Return the Steps object filled with the loaded data
}

// This function saves the steps to a specified file.
void Steps::saveSteps(const std::string& filename) const {
	// Open the steps file for writing, truncating any existing content
	std::ofstream steps_file(filename, std::ios::trunc);

	// Check for opening error
	if (!steps_file.is_open()) { 
		std::cout << "Error: Could not open steps file " << filename << std::endl;
		return;
	}

	// Write the difficulty level and random seed to the file
	steps_file << difficulty << '\n';
	steps_file << randomSeed << '\n' << steps.size(); // Write the number of steps

	// Write each step to the file
	for (const auto& step : steps) {
		steps_file << '\n' << step.first << ' ' << step.second;
	}
	steps_file.close();// Close the file after writing
}
