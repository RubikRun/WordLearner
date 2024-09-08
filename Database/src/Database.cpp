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
		// Open data file with an input stream
		std::ifstream dataFile(dataFilepath);
		if (!dataFile.is_open())
		{
			std::cout << "ERROR (Database): Cannot open words data file.\n";
			return;
		}
		// Read data file line by line
		std::string line;
		while (std::getline(dataFile, line))
		{
			// TODO: process line, add new word set to list
			std::cout << line << "\n";
		}
		// Close data file stream
		dataFile.close();
		if (dataFile.is_open())
		{
			std::cout << "ERROR (Database): Cannot close words data file.\n";
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

	std::string Database::serializeWord(const Word& word) const
	{
		const char& sep = m_separators[0];
		return std::to_string(word.id) + sep + word.termA + sep + word.termB + sep + word.note;
	}

	void Database::printWords() const
	{
		for (const Word& word : m_words)
		{
			std::cout << serializeWord(word) << "\n";
		}
	}

} // namespace WordLearner