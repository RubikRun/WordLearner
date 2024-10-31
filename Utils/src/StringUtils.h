#pragma once

#include <string>

namespace WordLearner
{

	namespace StringUtils
	{
		// Breaks a given string into multiple lines.
		// Allows a certain maximum length of a line. This maximum length might be disobeyed if there are tokens longer than that.
		// Always inserts a new line in between tokens, not breaking up any token.
		// "Token" here means a substring of non-whitespace characters that cannot be extended to the left or right.
		std::string breakIntoMultipleLines(const std::string str, int maxLineLength = 100);

	} // namespace StringUtils

} // namespace WordLearner
