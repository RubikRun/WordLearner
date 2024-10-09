#define WL_FILENAME "Database.cpp"

#include "Database.h"

#include <fstream>
#include <sstream>

#include "Logger.hpp"

static bool DO_DEBUG_DATABASE = true;

namespace WordLearner {

	void Database::loadDatabase()
	{
		loadSeparators();
		loadWords("Database/data/words.data");
		loadWordSets("Database/data/word_sets.data");
	}

	void Database::exportDatabase()
	{
		exportWords("Database/data/words.data");
		exportWordSets("Database/data/word_sets.data");
	}

	bool Database::addWord(const Word& word)
	{
		// Check if given word's ID is valid
		if (word.id <= 0)
		{
			WL_LOG_ERRORF("Trying to add a word with an invalid ID.");
			return false;
		}
		// Check if given word's ID is unique
		if (idExists(word.id))
		{
			WL_LOG_ERRORF("Trying to add a word with an already existing ID.");
			return false;
		}
		// Add new word to database's list of words
		m_words.push_back(word);
		// Add new word to global word set
		if (m_globalWordSetIndex < 0 || m_globalWordSetIndex >= m_wordSets.size())
		{
			WL_LOG_ERRORF("Trying to add a new word but global word set is not initialized.");
			return false;
		}
		m_wordSets[m_globalWordSetIndex].words.push_back(word.id);
		return true;
	}

	std::vector<Word> Database::getWordsFromWordSet(int wordSetId) const
	{
		std::vector<Word> wordsFromWordSet;
		// Find word set by given ID
		const WordSet* wordSet = findWordSet(wordSetId);
		if (wordSet == nullptr)
		{
			WL_LOG_ERRORF("Trying to get words of a word set that doesn't exist, with ID = " << wordSetId);
			return wordsFromWordSet;
		}
		// Reserve memory for final words list
		wordsFromWordSet.reserve(wordSet->words.size());
		// Traverse word IDs of word set
		for (int wordId : wordSet->words)
		{
			// Find word by ID
			const Word* word = findWord(wordId);
			if (word == nullptr)
			{
				WL_LOG_ERRORF("Invalid word ID #" << wordId << " in word set #" << wordSetId);
				continue;
			}
			// Add word to final list
			wordsFromWordSet.push_back(*word);
		}
		return wordsFromWordSet;
	}

	void Database::loadWords(const std::string& dataFilepath)
	{
		if (m_separators.empty())
		{
			WL_LOG_ERRORF("Cannot load words before loading separators.");
			return;
		}
		// Open data file with an input stream
		std::ifstream dataFile(dataFilepath);
		if (!dataFile.is_open())
		{
			WL_LOG_ERRORF("Cannot open words data file for load.");
			return;
		}
		// Read first line which is the number of words in the file
		std::string line;
		int wordsCount = -1;
		if (std::getline(dataFile, line))
		{
			try
			{
				wordsCount = int(std::stoul(line));
			}
			catch (...)
			{
				WL_LOG_WARNINGF("Words data file's first line is not a valid number. It should be number of objects in file.");
			}
		}
		else
		{
			WL_LOG_ERRORF("Words data file is empty.");
			dataFile.close();
			return;
		}
		// Allocate memory for that many words
		m_words.clear();
		if (wordsCount != -1)
		{
			m_words.reserve(wordsCount);
		}
		// Read data file line by line
		int wordsProcessed = 0;
		int lineIdx = 1;
		while (std::getline(dataFile, line))
		{
			lineIdx++;
			// Parse word from line
			Word word;
			if (!parseWord(line, lineIdx, word))
			{
				WL_LOG_ERRORF("Invalid word on line " << lineIdx << ".");
				continue;
			}
			// Check if word's ID is unique
			if (idExists(word.id))
			{
				WL_LOG_ERRORF("Word on line " << lineIdx << " has a duplicate ID with an already existing object.");
				continue;
			}
			// Add word to database's list of words
			m_words.push_back(word);
			wordsProcessed++;
		}
		// Log info about loaded words
		WL_LOG_INFOF("Successfully loaded " << wordsProcessed << " words.");
		if (DO_DEBUG_DATABASE)
		{
			printWords();
		}
		// Check if the words we processed are as many as the file says
		if (wordsCount != -1 && wordsProcessed != wordsCount)
		{
			WL_LOG_WARNINGF("Loaded words from data file are not as many as the first line says.");
		}
		// Close data file stream
		dataFile.close();
		if (dataFile.is_open())
		{
			WL_LOG_ERRORF("Cannot close words data file after load.");
		}
	}

	void Database::loadWordSets(const std::string& dataFilepath)
	{
		if (m_separators.empty())
		{
			WL_LOG_ERRORF("Cannot load word sets before loading separators.");
			return;
		}
		if (m_separators.size() < 2)
		{
			WL_LOG_ERRORF("Cannot load word sets because we have loaded only 1 separator, we need at least 2.");
			return;
		}
		// Open data file with an input stream
		std::ifstream dataFile(dataFilepath);
		if (!dataFile.is_open())
		{
			WL_LOG_ERRORF("Cannot open word sets data file for load.");
			return;
		}
		// Read first line which is the number of word sets in the file
		std::string line;
		int wordSetsCount = -1;
		if (std::getline(dataFile, line))
		{
			try
			{
				wordSetsCount = int(std::stoul(line));
			}
			catch (...)
			{
				WL_LOG_WARNINGF("Word sets data file's first line is not a valid number. It should be number of objects in file.");
			}
		}
		else
		{
			WL_LOG_ERRORF("Word sets data file is empty.");
			dataFile.close();
			return;
		}
		// Allocate memory for that many word sets + 1 for the global word set
		m_wordSets.clear();
		if (wordSetsCount != -1)
		{
			m_wordSets.reserve(wordSetsCount);
		}
		// Create global word set containing all loaded words
		createGlobalWordSet();
		// Read data file line by line
		int wordSetsProcessed = 0;
		int lineIdx = 1;
		while (std::getline(dataFile, line))
		{
			lineIdx++;
			// Parse word set from line
			WordSet wordSet;
			if (!parseWordSet(line, lineIdx, wordSet))
			{
				WL_LOG_ERRORF("Invalid word set on line " << lineIdx << ".");
				continue;
			}
			// Check if word set's ID is unique
			if (idExists(wordSet.id))
			{
				WL_LOG_ERRORF("Word set on line " << lineIdx << " has a duplicate ID with an already existing object.");
				continue;
			}
			// Add word set to database's list of word sets
			m_wordSets.push_back(wordSet);
			wordSetsProcessed++;
		}
		// Log info about loaded word sets
		WL_LOG_INFOF("Successfully loaded " << wordSetsProcessed << " word sets.");
		if (DO_DEBUG_DATABASE)
		{
			printWordSets();
		}
		// Check if the word sets we processed are as many as the file says
		if (wordSetsCount != -1 && wordSetsProcessed != wordSetsCount)
		{
			WL_LOG_WARNINGF("Loaded word sets from data file are not as many as the first line says.");
		}
		// Close data file stream
		dataFile.close();
		if (dataFile.is_open())
		{
			WL_LOG_ERRORF("Cannot close word sets data file after load.");
		}
	}

	void Database::loadSeparators()
	{
		// Open data file with an input stream
		std::ifstream dataFile("Database/data/separators.data");
		if (!dataFile.is_open())
		{
			WL_LOG_ERRORF("Cannot open separators data file for load.");
			return;
		}
		// Read separators from data file.
		// They are supposed to be on a single line, not separated by anything,
		// so we can directly read them
		dataFile >> m_separators;
		if (m_separators.empty())
		{
			WL_LOG_ERRORF("Missing separators in data file.");
		}
		// Close data file stream
		dataFile.close();
		if (dataFile.is_open())
		{
			WL_LOG_ERRORF("Cannot close separators data file after load.");
		}
	}

	bool Database::parseWord(const std::string& decl, int lineIdx, Word& word) const
	{
		// Create a string stream containing the declaration string.
		std::stringstream declStream(decl);
		// Read word's properties one by one
		// by separating the declaration string with our first separator
		//
		// - Read word's ID
		std::string propertyStr;
		if (std::getline(declStream, propertyStr, m_separators[0]))
		{
			try
			{
				word.id = int(std::stoul(propertyStr));
			}
			catch (...)
			{
				WL_LOG_ERRORF("Word on line " << lineIdx << " has an invalid ID.");
				return false;
			}
		}
		else
		{
			WL_LOG_ERRORF("Word on line " << lineIdx << " is empty.");
			return false;
		}
		// - Read word's term A
		if (!std::getline(declStream, word.termA, m_separators[0]))
		{
			WL_LOG_ERRORF("Word on line " << lineIdx << " is missing its second propery - term A.");
			return false;
		}
		// - Read word's term B
		if (!std::getline(declStream, word.termB, m_separators[0]))
		{
			WL_LOG_ERRORF("Word on line " << lineIdx << " is missing its third propery - term B.");
			return false;
		}
		// - Read word's note
		std::getline(declStream, word.note, m_separators[0]);
		// At this point we successfully read all the properties
		return true;
	}

	bool Database::parseWordSet(const std::string& decl, int lineIdx, WordSet& wordSet) const
	{
		// Create a string stream containing the declaration string.
		std::stringstream declStream(decl);
		// Read word set's properties one by one
		// by separating the declaration string with our first separator
		//
		// - Read word set's ID
		std::string propertyStr;
		if (std::getline(declStream, propertyStr, m_separators[0]))
		{
			try
			{
				wordSet.id = int(std::stoul(propertyStr));
			}
			catch (...)
			{
				WL_LOG_ERRORF("Word set on line " << lineIdx << " has an invalid ID.");
				return false;
			}
		}
		else
		{
			WL_LOG_ERRORF("Word set on line " << lineIdx << " is empty.");
			return false;
		}
		// - Read word set's name
		if (!std::getline(declStream, wordSet.name, m_separators[0]))
		{
			WL_LOG_ERRORF("Word set on line " << lineIdx << " is missing its second propery - name.");
			return false;
		}
		// Read word set's list of words
		std::getline(declStream, propertyStr, m_separators[0]);
		if (!parseIntList(propertyStr, wordSet.words))
		{
			WL_LOG_ERRORF("Word set on line " << lineIdx << " has an invalid list of words.");
			return false;
		}
		// Check if each ID in the list is an ID of an existing word
		for (int id : wordSet.words)
		{
			if (findWord(id) == nullptr)
			{
				WL_LOG_ERRORF("Word set on line " << lineIdx << " references a non-existing word.");
				return false;
			}
		}
		// At this point we successfully read all the properties
		return true;
	}

	void Database::createGlobalWordSet()
	{
		// Construct global word set with all loaded words
		WordSet globalWordSet;
		globalWordSet.id = 0;
		globalWordSet.name = "global";
		// Allocate memory for words, as many as we have loaded
		globalWordSet.words.reserve(m_words.size());
		// Construct list of IDs of loaded words
		for (const Word& word : m_words)
		{
			globalWordSet.words.push_back(word.id);
		}
		// Add global word set to list of word sets
		m_globalWordSetIndex = int(m_wordSets.size());
		m_wordSets.push_back(globalWordSet);
	}

	bool Database::parseIntList(const std::string& decl, std::vector<int>& list) const
	{
		list.clear();
		// Create a string stream containing the declaration string.
		std::stringstream declStream(decl);
		// Read first element from list by separating the declaration string with our second separator.
		// First element in list is the number of elements (not counting itself)
		std::string elemStr;
		int elemCount = -1;
		if (std::getline(declStream, elemStr, m_separators[1]))
		{
			// Parse element string into an integer
			try
			{
				elemCount = std::stoi(elemStr);
			}
			catch (...)
			{
				return false;
			}
		}
		// Allocate memory for that many elements
		list.clear();
		if (elemCount > -1)
		{
			list.reserve(elemCount);
		}
		else
		{
			WL_LOG_WARNINGF("Invalid number of elements provided in the beginning of a list of integers, should be >= 0.");
		}
		// Read list's elements one by one (actual elements, after the first element which is the count)
		// by separating the declaration string with our second separator
		while (std::getline(declStream, elemStr, m_separators[1]))
		{
			// Parse element string into an integer
			int elem;
			try
			{
				elem = std::stoi(elemStr);
			}
			catch (...)
			{
				return false;
			}
			// Add integer element to list
			list.push_back(elem);
		}
		// Check if processed elements are as many as the count written in list's beginning.
		if (list.size() != elemCount)
		{
			WL_LOG_WARNINGF("Elements in a list of integers are NOT as many as the count in list's beginning.");
		}
		// At this point we have successfully gone through all elements, parsed them and added them to list
		return true;
	}

	void Database::exportWords(const std::string& dataFilepath)
	{
		// Open data file with an output stream
		std::ofstream dataFile(dataFilepath);
		if (!dataFile.is_open())
		{
			WL_LOG_ERRORF("Cannot open words data file for export.");
			return;
		}
		// Write the number of words to the file
		dataFile << m_words.size() << "\n";
		// Serialize each word and write it to the file
		for (const Word& word : m_words)
		{
			const std::string wordSerialized = serializeWord(word);
			dataFile << wordSerialized << "\n";
		}
		// Close data file stream
		dataFile.close();
		if (dataFile.is_open())
		{
			WL_LOG_ERRORF("Cannot close words data file after export.");
		}
	}

	void Database::exportWordSets(const std::string& dataFilepath)
	{
		// Open data file with an output stream
		std::ofstream dataFile(dataFilepath);
		if (!dataFile.is_open())
		{
			WL_LOG_ERRORF("Cannot open word sets data file for export.");
			return;
		}
		// Write number of word sets to the file, -1 because we don't export global word set
		dataFile << m_wordSets.size() - 1 << "\n";
		// Serialize each word set and write it to the file
		int wordSetsExported = 0;
		for (const WordSet& wordSet: m_wordSets)
		{
			// Skip global word set
			if (wordSet.id == 0)
			{
				continue;
			}
			const std::string wordSetSerialized = serializeWordSet(wordSet);
			dataFile << wordSetSerialized << "\n";
			wordSetsExported++;
		}
		// Check if we exported the expected number of word sets
		if (wordSetsExported != m_wordSets.size() - 1)
		{
			WL_LOG_ERRORF("Exported an unexpected number of word sets to data file. Might be because of a missing global word set.");
		}
		// Close data file stream
		dataFile.close();
		if (dataFile.is_open())
		{
			WL_LOG_ERRORF("Cannot close word sets data file after export.");
		}
	}

	std::string Database::serializeWord(const Word& word) const
	{
		const char& sep = m_separators[0];
		return std::to_string(word.id) + sep + word.termA + sep + word.termB + sep + word.note;
	}

	std::string Database::serializeWordSet(const WordSet& wordSet) const
	{
		const char& sep = m_separators[0];
		return std::to_string(wordSet.id) + sep + wordSet.name + sep + serializeIntList(wordSet.words);
	}

	std::string Database::serializeIntList(const std::vector<int>& list) const
	{
		if (list.empty())
		{
			return std::string();
		}
		const char& sep = m_separators[1];
		std::stringstream lStream;
		lStream << list.size();
		for (int elem : list)
		{
			lStream << sep << elem;
		}
		return lStream.str();
	}

	const Word* Database::findWord(int id) const
	{
		for (const Word& word : m_words)
		{
			if (word.id == id)
			{
				return &word;
			}
		}
		return nullptr;
	}

	const WordSet* Database::findWordSet(int id) const
	{
		for (const WordSet& wordSet : m_wordSets)
		{
			if (wordSet.id == id)
			{
				return &wordSet;
			}
		}
		return nullptr;
	}

	bool Database::idExists(int id) const
	{
		return findWord(id) != nullptr || findWordSet(id) != nullptr;
	}

	void Database::printWords() const
	{
		for (const Word& word : m_words)
		{
			WL_LOG_INFOF(serializeWord(word));
		}
		WL_LOG_INFOF("");
	}

	void Database::printWordSets() const
	{
		for (const WordSet& wordSet : m_wordSets)
		{
			WL_LOG_INFOF(serializeWordSet(wordSet));
		}
		WL_LOG_INFOF("");
	}

} // namespace WordLearner

#undef WL_FILENAME