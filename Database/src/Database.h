#pragma once

#include "Word.h"
#include "WordSet.h"

#include <vector>
#include <string>

namespace WordLearner {

	// A class representing the application's database
	class Database
	{
	public:
		// TODO: comment
		void load();

	private: /* functions */
		// TODO: comment
		void loadWords(const std::string& dataFilepath);
		// TODO: comment
		void loadWordSets(const std::string& dataFilepath);
		// TODO: comment
		void loadSeparators(const std::string& dataFilepath);

	private: /* variables */
		// A list of all words in the database
		std::vector<Word> m_words;
		// A list of all word sets in the database
		std::vector<WordSet> m_wordSets;
		// A string containing the separators used in data files
		// to separate object properties and list elements
		std::string separators;
	};

} // namespace WordLearner