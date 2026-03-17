#ifndef Results_H
#define Results_H

#include <list>

class Results {
public:
	enum ResultValue { die, finished, noResult, useHammer };
private:
	std::list<std::pair<size_t, ResultValue>> results; // pair: iteration, result
public:
	// Loads the results from a file
	static Results loadResults(const std::string& filename);

	// Saves the results to a file
	void saveResults(const std::string& filename) const;

	// Adds a result to the results list
	void addResult(size_t iteration, ResultValue result) {
		results.push_back({ iteration, result });
	}

	// Getter to get the results list
	std::list<std::pair<size_t, ResultValue>> getResults() const {
		return results;
	}

	// Getter to pop a result (return and remove the last result)
	std::pair<size_t, ResultValue> popResult() {
		if (results.empty()) return { 0, Results::noResult };
		auto result = results.front();
		results.pop_front();
		return result;
	}

	
	// Reports an error in the results (for debugging purposes)
	void reportResultError(const std::string& message, const std::string& filename, size_t iteration, size_t result_it);

};

#endif 
