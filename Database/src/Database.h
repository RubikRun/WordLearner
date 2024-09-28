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
		// Loads the entire database from its .data files
		void load();

	private: /* functions */
		// Loads words from a given .data file
		void loadWords(const std::string& dataFilepath);
		// Loads word sets from a given .data file
		void loadWordSets(const std::string& dataFilepath);
		// Loads database's separators from their .data file
		void loadSeparators();

		// Parses a word from a declaration line from a .data file.
		// Returns true on success.
		bool parseWord(const std::string& decl, int lineIdx, Word& word) const;
		// Parses a word set from a declaration line from a .data file.
		// Returns true on success.
		bool parseWordSet(const std::string& decl, int lineIdx, WordSet& wordSet) const;

		// Parses a list of integers from a declaration from a .data file.
		// Returns true on success.
		bool parseIntList(const std::string& decl, std::vector<int>& list) const;

		// Serializes a given word into a declaration line for a .data file
		std::string serializeWord(const Word& word) const;
		// Serializes a given word set into a declaration line for a .data file
		std::string serializeWordSet(const WordSet& wordSet) const;
		// Serializes a given list of integers into a declaration string for a .data file
		std::string serializeIntList(const std::vector<int>& list) const;

		// Finds word with a given ID in database
		// @param[in] id - ID of the word to find
		// @return Pointer to the found word. Nullptr if word is not found.
		const Word* findWord(int id) const;
		// Finds word set with a given ID in database
		// @param[in] id - ID of the word set to find
		// @return Pointer to the found word set. Nullptr if word set is not found.
		const WordSet* findWordSet(int id) const;

		// Prints all loaded words to the console
		void printWords() const;
		// Prints all loaded word sets to the console
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