#pragma once

#include "Word.h"

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <functional>

namespace WordLearner
{
	class Database;
	class WordsTableWidget;

	// Widget for showing, editing, creating and deleting words from a given database.
	// Consists of a table containing the words and a button for creating a new word.
	class WordsWidget : public QWidget
	{
	public:
		// Creates a words widget for the words of a given database
		WordsWidget(Database& database, std::function<int()> getSelectedWordSetIdCallback, QWidget* parent = nullptr);

		// Updates words widget to contain the given list of words. Removes all previously added words.
		void update(const std::vector<Word>& words);

	private: /* functions */

		// Creates all UI elements of widget
		void createUi();

	private slots:

		// Slot that's called when the "Create new word" button is pressed
		void onCreateWordButtonPressed();

		// Slot that's called when a new word must be created.
		// It's called with the new word's data.
		void onCreateWord(const std::string& termA, const std::string& termB, const std::string& note);

	private: /* variables */

		// Struct for holding together widget's UI elements
		struct Ui
		{
			// Layout of words widget
			QVBoxLayout* layout = nullptr;
			// Table widget for showing a list of words
			WordsTableWidget* wordsTableWidget = nullptr;
			// Button for creating a new word
			QPushButton* createWordButton = nullptr;
		} ui;

		// Callback function to retrieve ID of currently selected word set
		std::function<int()> m_getSelectedWordSetIdCallback;

		Database& m_database;
	};

} // namespace WordLearner