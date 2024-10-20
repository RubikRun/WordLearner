#define WL_FILENAME "ResourceManager.cpp"

#include "ResourceManager.h"

#include "Logger.hpp"

#include <fstream>
#include <sstream>

using namespace WordLearner;

static const char* FILEPATH_LIST_WIDGET_STYLESHEET = "WordLearner/resources/ListWidget.css";
static const char* FILEPATH_TABLE_WIDGET_STYLESHEET = "WordLearner/resources/TableWidget.css";

static std::string listWidgetStylesheet;
static std::string tableWidgetStylesheet;

static void loadListWidgetStylesheet()
{
	// Open CSS file with an input stream
	std::ifstream cssFile(FILEPATH_LIST_WIDGET_STYLESHEET);
	if (!cssFile.is_open())
	{
		WL_LOG_ERRORF("Cannot open list widget CSS file for load.");
		return;
	}
	// Read entire file into a single string
	std::ostringstream stringStream;
	stringStream << cssFile.rdbuf();
	listWidgetStylesheet = stringStream.str();
	// Close CSS file stream
	cssFile.close();
	if (cssFile.is_open())
	{
		WL_LOG_ERRORF("Cannot close list widget CSS file after load.");
	}
}

static void loadTableWidgetStylesheet()
{
	// Open CSS file with an input stream
	std::ifstream cssFile(FILEPATH_TABLE_WIDGET_STYLESHEET);
	if (!cssFile.is_open())
	{
		WL_LOG_ERRORF("Cannot open table widget CSS file for load.");
		return;
	}
	// Read entire file into a single string
	std::ostringstream stringStream;
	stringStream << cssFile.rdbuf();
	tableWidgetStylesheet = stringStream.str();
	// Close CSS file stream
	cssFile.close();
	if (cssFile.is_open())
	{
		WL_LOG_ERRORF("Cannot close table widget CSS file after load.");
	}
}

void ResourceManager::load()
{
	loadListWidgetStylesheet();
	loadTableWidgetStylesheet();
}

const std::string& ResourceManager::getListWidgetStylesheet()
{
	return listWidgetStylesheet;
}

const std::string& WordLearner::ResourceManager::getTableWidgetStylesheet()
{
	return tableWidgetStylesheet;
}
