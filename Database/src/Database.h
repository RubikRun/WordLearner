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

		// TODO: comment
		bool parseWord(const std::string& decl, int lineIdx, Word& word) const;
		// TODO: comment
		bool parseWordSet(const std::string& decl, int lineIdx, WordSet& wordSet) const;

		// TODO: comment
		bool parseListOfInt(const std::string& decl, std::vector<int>& list) const;

		// TODO: comment
		std::string serializeWord(const Word& word) const;
		// TODO: comment
		std::string serializeWordSet(const WordSet& wordSet) const;
		// TODO: comment
		std::string serializeListInt(const std::vector<int>& list) const;

		// Finds word with a given ID in database
		// @param[in] id - ID of the word to find
		// @return Pointer to the found word. Nullptr if word is not found.
		const Word* findWord(int id) const;
		// Finds word set with a given ID in database
		// @param[in] id - ID of the word set to find
		// @return Pointer to the found word set. Nullptr if word set is not found.
		const WordSet* findWordSet(int id) const;

		// TODO: comment
		void printWords() const;
		// TODO: comment
		void printWordSets() const;

	private: /* variables */
		// A list of all words in the database
		std::vector<Word> m_words;
		// A list of all word sets in the database
		std::vector<WordSet> m_wordSets;
		// A string containing the separators used in data files
		// to separate object properties and list elements
		std::string m_separators;
	};

} // namespace WordLearner