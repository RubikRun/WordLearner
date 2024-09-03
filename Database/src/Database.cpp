#include "Database.h"

#include <fstream>
#include <iostream>

namespace WordLearner {

	void Database::load()
	{
		loadSeparators("Database/data/separators.data");
		loadWords("Database/data/words.data");
		loadWordSets("Database/data/word_sets.data");
	}

	void Database::loadWords(const std::string& dataFilepath)
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
			// TODO: process line, add new word to list
			std::cout << line << "\n";
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
		dataFile >> separators;
		if (separators.empty())
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

} // namespace WordLearner