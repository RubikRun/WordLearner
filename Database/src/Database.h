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
		void loadDatabase();
		// Exports the entire database to its .data files
		void exportDatabase();

		// Returns list of currently loaded words
		const std::vector<Word>& getWords() const
		{
			return m_words;
		}
		// Returns list of currently loaded word sets
		const std::vector<WordSet>& getWordSets() const
		{
			return m_wordSets;
		}

		// Creates a new word in database from given data.
		// Adds new word to global word set.
		// Returns created word's ID.
		int createWord(const std::string& termA, const std::string& termB, const std::string& note);
		// Creates a new word set in database, with given name.
		// Returns created word set's ID.
		int createWordSet(const std::string& name);

		// Edits a word set by setting it a given new name.
		// Returns true on success.
		bool editWordSetName(int wordSetId, const std::string& newName);
		// Edits a word by setting it a given new term A.
		// Returns true on success.
		bool editWordTermA(int wordId, const std::string& newTermA);
		// Edits a word by setting it a given new term B.
		// Returns true on success.
		bool editWordTermB(int wordId, const std::string& newTermB);
		// Edits a word by setting it a given new note.
		// Returns true on success.
		bool editWordNote(int wordId, const std::string& newNote);

		// Adds a given word to a given word set.
		// Returns true on success.
		bool addWordToWordSet(int wordId, int wordSetId);
		// Returns list of words from a given word set.
		// @param[in] wordSetId - ID of word set whose words list we want to retrieve
		// @return List of words contained in the word set.
		std::vector<Word> getWordsFromWordSet(int wordSetId) const;

	private: /* functions */
		// Adds a new word to database, and keeps track of max ID.
		// Does NOT add word to global word set.
		// Returns true on success.
		bool addWord(const Word& word);
		// Adds a new word set to database, and keeps track of max ID.
		// Returns true on success.
		bool addWordSet(const WordSet& wordSet);

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

		// Creates a global word set containing all currently loaded words.
		// Global word set always has an ID=0 and name="global".
		// Adds global word set to the list of word sets.
		void createGlobalWordSet();

		// Parses a list of integers from a declaration from a .data file.
		// Returns true on success.
		bool parseIntList(const std::string& decl, std::vector<int>& list) const;

		// Exports words to a given .data file
		void exportWords(const std::string& dataFilepath);
		// Exports word sets to a given .data file
		void exportWordSets(const std::string& dataFilepath);

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
		Word* findWord(int id);
		// Finds word set with a given ID in database
		// @param[in] id - ID of the word set to find
		// @return Pointer to the found word set. Nullptr if word set is not found.
		const WordSet* findWordSet(int id) const;
		WordSet* findWordSet(int id);

		// Checks if given ID exists across all object types
		bool idExists(int id) const;
		// Generates a new non-existing ID that can be used for a new object
		int getNewId() const;
		// Updates max ID with a given ID of a new object
		void updateMaxId(int newId);

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

		// Index of global word set in the list of word sets
		int m_globalWordSetIndex = -1;

		// Current max ID across all objects in database
		int m_maxId = 0;
	};

} // namespace WordLearner