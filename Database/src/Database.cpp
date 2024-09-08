#include "Database.h"

#include <fstream>
#include <sstream>
#include <iostream>

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
			std::cout << "ERROR (Database): Cannot load words before loading separators.\n";
			return;
		}
		// Open data file with an input stream
		std::ifstream dataFile(dataFilepath);
		if (!dataFile.is_open())
		{
			std::cout << "ERROR (Database): Cannot open words data file.\n";
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
				std::cout << "WARNING (Database): Words data file's first line is not a valid number. It should be number of objects in file.\n";
			}
		}
		else
		{
			std::cout << "ERROR (Database): Words data file is empty.\n";
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
			// Parse word from line
			Word word;
			if (parseWord(line, lineIdx, word))
			{
				// Add word to database's list of words
				m_words.push_back(word);
				// Count number of processed words
				wordsProcessed++;
			}
			else
			{
				std::cout << "ERROR (Database): Invalid word on line " << lineIdx << ".\n";
			}
			lineIdx++;
		}
		// Log info about loaded words
		std::cout << "INFO (Database): Successfully loaded " << wordsProcessed << " words.\n";
		if (DO_DEBUG_DATABASE)
		{
			printWords();
		}
		// Check if the words we processed are as many as the file says
		if (wordsCount != -1 && wordsProcessed != wordsCount)
		{
			std::cout << "WARNING (Database): Words in data file are not as many as the first line says.\n";
		}
		// Close data file stream
		dataFile.close();
		if (dataFile.is_open())
		{
			std::cout << "ERROR (Database): Cannot close words data file.\n";
		}
	}

	void Database::loadWordSets(const std::string& dataFilepath)
	{
		if (m_separators.empty())
		{
			std::cout << "ERROR (Database): Cannot load word sets before loading separators.\n";
			return;
		}
		if (m_separators.size() < 2)
		{
			std::cout << "ERROR (Database): Cannot load word sets because we have loaded only 1 separator, we need at least 2.\n";
			return;
		}
		// Open data file with an input stream
		std::ifstream dataFile(dataFilepath);
		if (!dataFile.is_open())
		{
			std::cout << "ERROR (Database): Cannot open word sets data file.\n";
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
				std::cout << "WARNING (Database): Word sets data file's first line is not a valid number. It should be number of objects in file.\n";
			}
		}
		else
		{
			std::cout << "ERROR (Database): Word sets data file is empty.\n";
			dataFile.close();
			return;
		}
		// Allocate memory for that many word sets
		m_words.clear();
		if (wordSetsCount != -1)
		{
			m_wordSets.reserve(wordSetsCount);
		}
		// Read data file line by line
		int wordSetsProcessed = 0;
		int lineIdx = 1;
		while (std::getline(dataFile, line))
		{
			// Parse word set from line
			WordSet wordSet;
			if (parseWordSet(line, lineIdx, wordSet))
			{
				// Add word set to database's list of word sets
				m_wordSets.push_back(wordSet);
				// Count number of processed word sets
				wordSetsProcessed++;
			}
			else
			{
				std::cout << "ERROR (Database): Invalid word set on line " << lineIdx << ".\n";
			}
			lineIdx++;
		}
		// Log info about loaded word sets
		std::cout << "INFO (Database): Successfully loaded " << wordSetsProcessed << " word sets.\n";
		if (DO_DEBUG_DATABASE)
		{
			printWordSets();
		}
		// Check if the word sets we processed are as many as the file says
		if (wordSetsCount != -1 && wordSetsProcessed != wordSetsCount)
		{
			std::cout << "WARNING (Database): Word sets in data file are not as many as the first line says.\n";
		}
		// Close data file stream
		dataFile.close();
		if (dataFile.is_open())
		{
			std::cout << "ERROR (Database): Cannot close word sets data file.\n";
		}
	}

	void Database::loadSeparators(const std::string& dataFilepath)
	{
		// Open data file with an input stream
		std::ifstream dataFile(dataFilepath);
		if (!dataFile.is_open())
		{
			std::cout << "ERROR (Database): Could not open separators data file.\n";
			return;
		}
		// Read separators from data file.
		// They are supposed to be on a single line, not separated by anything,
		// so we can directly read them
		dataFile >> m_separators;
		if (m_separators.empty())
		{
			std::cout << "ERROR (Database): Missing separators in data file.\n";
		}
		// Close data file stream
		dataFile.close();
		if (dataFile.is_open())
		{
			std::cout << "ERROR (Database): Could not close separators data file.\n";
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
				std::cout << "ERROR (Database): Word on line " << lineIdx << " has an invalid ID.\n";
				return false;
			}
		}
		else
		{
			std::cout << "ERROR (Database): Word on line " << lineIdx << " is empty.\n";
			return false;
		}
		// - Read word's term A
		if (!std::getline(declStream, word.termA, m_separators[0]))
		{
			std::cout << "ERROR (Database): Word on line " << lineIdx << " is missing its second propery - term A.\n";
			return false;
		}
		// - Read word's term B
		if (!std::getline(declStream, word.termB, m_separators[0]))
		{
			std::cout << "ERROR (Database): Word on line " << lineIdx << " is missing its third propery - term B.\n";
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
				std::cout << "ERROR (Database): Word set on line " << lineIdx << " has an invalid ID.\n";
				return false;
			}
		}
		else
		{
			std::cout << "ERROR (Database): Word set on line " << lineIdx << " is empty.\n";
			return false;
		}
		// - Read word set's name
		if (!std::getline(declStream, wordSet.name, m_separators[0]))
		{
			std::cout << "ERROR (Database): Word set on line " << lineIdx << " is missing its second propery - name.\n";
			return false;
		}
		// Read word set's list of words
		std::getline(declStream, propertyStr, m_separators[0]);
		if (!parseListOfInt(propertyStr, wordSet.words))
		{
			std::cout << "ERROR (Database): Word set on line " << lineIdx << " has an invalid list of words.\n";
			return false;
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

	void Database::printWords() const
	{
		for (const Word& word : m_words)
		{
			std::cout << serializeWord(word) << "\n";
		}
	}

	void Database::printWordSets() const
	{
		for (const WordSet& wordSet : m_wordSets)
		{
			std::cout << serializeWordSet(wordSet) << "\n";
		}
	}

} // namespace WordLearner