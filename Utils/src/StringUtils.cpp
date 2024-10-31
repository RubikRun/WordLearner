#define WL_FILENAME "StringUtils.cpp"

#include "StringUtils.h"

#include "Logger.hpp"
#include <sstream>
#include <vector>

namespace WordLearner
{

	namespace StringUtils
	{
		std::string breakIntoMultipleLines(const std::string str, int maxLineLen)
		{
			// If the whole string is shorter than the maximum line length,
			// then it's short enough to be a single line, and we are done.
			if (str.length() <= maxLineLen)
			{
				return str;
			}

			// Create an output string stream where we will push the resulting string token by token
			std::ostringstream oss;
			// Create an input string stream from which we will read the given string token by token
			std::istringstream iss(str);
			// Currently processed token and length of current line
			std::string token;
			int currLineLen = 0;
			// Read tokens one by one from the string stream
			while (iss >> token)
			{
				const int tokenLen = int(token.length());
				// If this token is the first on current line
				if (currLineLen == 0)
				{
					// and it's short enough
					if (tokenLen <= maxLineLen)
					{
						// then just add it normally
						oss << token;
						currLineLen = tokenLen;
					}
					else
					{
						// If it's too long add it as a whole new line
						WL_LOG_WARNINGF("Trying to break a string into multiple lines, but there is a token longer than the maximum allowed line length.");
						oss << token << "\n";
					}
				}
				// If it's not the first on current line
				else
				{
					// and it's short enough
					if (currLineLen + 1 + tokenLen <= maxLineLen)
					{
						// then add it normally, with a space first
						oss << " ";
						oss << token;
						currLineLen += tokenLen;
					}
					else
					{
						// If it's too long, end current line and add it to next line
						oss << "\n";
						// If it's short enough
						if (tokenLen <= maxLineLen)
						{
							// add it normally to next line
							oss << token;
							currLineLen = tokenLen;
						}
						else
						{
							// If it's long enough make it be the whole next line
							WL_LOG_WARNINGF("Trying to break a string into multiple lines, but there is a token longer than the maximum allowed line length.");
							oss << token << "\n";
							currLineLen = 0;
						}
					}
				}
			}
			return oss.str();
		}

	} // namespace StringUtils

} // namespace WordLearner
