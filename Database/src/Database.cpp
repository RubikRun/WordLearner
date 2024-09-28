#define WL_FILENAME "Database.cpp"

#include "Database.h"

#include <fstream>
#include <sstream>

#include "Logger.hpp"

static bool DO_DEBUG_DATABASE = true;

namespace WordLearner {

	void Database::load()
	{
		loadSeparators("Database/data/separators.data");
		loadWords("Database/data/words.data");
		loadWordSets("Database/data/word_sets.data");
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
			WL_LOG_ERRORF("Cannot open words data file.");
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
			if (findWord(word.id) != nullptr)
			{
				WL_LOG_ERRORF("Word on line " << lineIdx << " has a duplicate ID with an already existing word.");
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
			WL_LOG_ERRORF("Cannot close words data file.");
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
			WL_LOG_ERRORF("Cannot open word sets data file.");
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
		// Allocate memory for that many word sets
		m_wordSets.clear();
		if (wordSetsCount != -1)
		{
			m_wordSets.reserve(wordSetsCount);
		}
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
			if (findWordSet(wordSet.id) != nullptr)
			{
				WL_LOG_ERRORF("Word set on line " << lineIdx << " has a duplicate ID with an already existing word set.");
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
			WL_LOG_ERRORF("Cannot close word sets data file.");
		}
	}

	void Database::loadSeparators(const std::string& dataFilepath)
	{
		// Open data file with an input stream
		std::ifstream dataFile(dataFilepath);
		if (!dataFile.is_open())
		{
			WL_LOG_ERRORF("Could not open separators data file.");
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
			WL_LOG_ERRORF("Could not close separators data file.");
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
		if (!parseListOfInt(propertyStr, wordSet.words))
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

	bool Database::parseListOfInt(const std::string& decl, std::vector<int>& list) const
	{
		list.clear();
		// Create a string stream containing the declaration string.
		std::stringstream declStream(decl);
		// Read list's elements one by one
		// by separating the declaration string with our second separator
		std::string elemStr;
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
		// At this point we have successfully gone through all elements, parsed them and added them to list
		return true;
	}

	std::string Database::serializeWord(const Word& word) const
	{
		const char& sep = m_separators[0];
		return std::to_string(word.id) + sep + word.termA + sep + word.termB + sep + word.note;
	}

	std::string Database::serializeWordSet(const WordSet& wordSet) const
	{
		const char& sep = m_separators[0];
		return std::to_string(wordSet.id) + sep + wordSet.name + sep + serializeListInt(wordSet.words);
	}

	std::string Database::serializeListInt(const std::vector<int>& list) const
	{
		if (list.empty())
		{
			return std::string();
		}
		const char& sep = m_separators[1];
		std::stringstream lStream(std::to_string(list[0]));
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