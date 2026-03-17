#include <limits>
#include <fstream>
#include <string>
#include <iostream>
#include <conio.h>

#include "Results.h"

// This function loads the results from a specified file.
Results Results::loadResults(const std::string& filename) {
	std::ifstream results_file(filename); // Open the results file for reading
	Results results;
	size_t size;

	if (!results_file.is_open()) // Check for opening error
	{
		std::cout << "Error: Could not open results file " << filename << std::endl; 
		return results; // Return an empty Results object
	}

	results_file >> size;  // Read the number of results from the file

	// Loop through each result
	while (!results_file.eof() && size-- != 0) {
		size_t iteration;  // Variable to store the iteration number
		int result;       // Variable to store the result value 
		results_file >> iteration >> result;  // Read the iteration and result from the file
		results.addResult(iteration, static_cast<ResultValue>(result));
	}
	results_file.close(); // Close the file after reading
	return results;  // Return the Results object filled with the loaded data
}

// This function saves the results to a specified file.
void Results::saveResults(const std::string& filename) const {
	// Open the results file for writing (truncate existing content)
	std::ofstream results_file(filename, std::ios::trunc);

	if (!results_file.is_open()) // Check for opening error 
	{
		std::cout << "Error: Could not open results file " << filename << std::endl;
		return;
	}

	results_file << results.size(); // Write the number of results to the file

	// Loop through each result in the results container
	for (const auto& result : results) {
		results_file << '\n' << result.first << ' ' << result.second; // Write iteration and result to file
	}
	results_file.close(); // Close the file after writing
}

// This function reports an error in the results and shows details.
void Results::reportResultError(const std::string& message, const std::string& filename, size_t iteration, size_t result_it) {
	system("cls");
	std::cout << "Screen " << filename << " - " << message << '\n';
	std::cout << "Game Iteration: " << iteration << '\n';
	std::cout << "Written Result Iteration: " << result_it << '\n';
	std::cout << "Press any key to continue to next screens (if any)" << std::endl;
	_getch();
}