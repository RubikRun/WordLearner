#pragma once

#include "Word.h"
#include "WordSet.h"

#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <functional>

namespace WordLearner
{
	class Database;
	class WordSetsListWidget;

	// Widget for showing, editing, creating and deleting word sets from a given database.
	// Consists of a list containing the word sets and a button for creating a new word set.
	class WordSetsWidget : public QWidget
	{
	public:
		// Creates an empty word sets widget. Must be initialized by calling init() later
		WordSetsWidget(QWidget* parent = nullptr);

		// Initializes word sets widget with word sets from a given database
		void init(Database* database, std::function<void(const std::vector<Word>& words)> updateWordsWidgetCallback);

		// Updates word sets widget to contain the given list of word sets. Removes all previously added word sets.
		void update(const std::vector<WordSet>& wordSets, int selectedWordSetId = -1);

		// Returns ID of word set currently selected in UI
		int getSelectedWordSetId() const;

	private: /* functions */

		// Creates all UI elements of widget
		void createUi();

	private slots:

		// Slot that's called when the "Create new word set" button is pressed
		void onCreateWordSetButtonPressed();

		// Slot that's called when a new word set must be created.
		// It's called with the new word set's data.
		void onCreateWordSet(const std::string& name);

	private: /* variables */

		// Struct for holding together widget's UI elements
		struct Ui
		{
			// Layout of word sets widget
			QVBoxLayout* layout = nullptr;
			// List widget for showing a list of word sets
			WordSetsListWidget* wordSetsListWidget = nullptr;
			// Button for creating a new word set
			QPushButton* createWordSetButton = nullptr;
		} ui;

		// Callback function to update words widget with a given list of words
		std::function<void(const std::vector<Word>& words)> m_updateWordsWidgetCallback;

		Database* m_database = nullptr;
	};

} // namespace WordLearner