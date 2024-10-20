#pragma once

#include <string>

namespace WordLearner
{

	// A namespace with functions for loading, managing, and providing access to
	// resources used by the application, for example images, fonts, style sheets, etc.
	namespace ResourceManager
	{

		// Loads all resources
		void load();

		// Returns the CSS stylesheet used for list widgets
		const std::string& getListWidgetStylesheet();

		// Returns the CSS stylesheet used for table widgets
		const std::string& getTableWidgetStylesheet();

	};

} // namespace WordLearner