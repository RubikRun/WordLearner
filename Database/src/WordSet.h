#pragma once

#include <string>
#include <vector>

namespace WordLearner
{
	// A struct representing a word set
	struct WordSet
	{
		// Word set's ID, unique across all objects in the application
		int id = -1;
		// Name of word set
		std::string name;
		// List of words contained in the word set, represented with their IDs
		std::vector<int> words;
	};

} // namespace WordLearner