#ifndef Steps_h
#define Steps_h
#pragma once

#include <list>

class Steps {
	int difficulty;
	long randomSeed = 0;
	std::list<std::pair<size_t, char>> steps; // pair: iteration, step
public:

	// Loads the steps from a file
	static Steps loadSteps(const std::string& filename);

	// Saves the steps to a file
	void saveSteps(const std::string& filename) const;

	// gets the random seed from saved steps
	long getRandomSeed() const {
		return randomSeed;
	}

	// sets the random seed 
	void setRandomSeed(long seed) {
		randomSeed = seed;
	}

	// Adds a step to the steps list
	void addStep(size_t iteration, char step) {
		steps.push_back({ iteration, step });
	}

	// Checks if the next step is on the specified iteration
	bool isNextStepOnIteration(size_t iteration) const {
		return !steps.empty() && steps.front().first == iteration;
	}

	// Pops a step (return and remove the first step)
	char popStep() {
		char step = steps.front().second;
		steps.pop_front();
		return step;
	}

	// Getter to get the steps list
	std::list<std::pair<size_t, char>> getSteps() const {
		return steps;
	}

	// Getter to get the next step iteration
	int getNextStep() const {
		return steps.front().first;
	}

	// Getter to get the difficulty level
	int getDifficulty() const {
		return difficulty;
	}

	// Setter to set the difficulty level
	void setDifficulty(int diff) {
		difficulty = diff;
	}

	
};

#endif
