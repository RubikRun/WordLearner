#pragma once

#include <string>

namespace WordLearner
{
	// A struct representing a word
	struct Word
	{
		// Word's ID, unique across all objects in the application
		int id = -1;
		// Word in language A
		std::string aTerm;
		// Word in language B
		std::string bTerm;
		// Additional note, explanation or example
		std::string note;
	};

} // namespace WordLearner