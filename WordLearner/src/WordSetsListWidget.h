#pragma once

#include "Word.h"
#include "WordSet.h"

#include <QtWidgets/QListWidget>
#include <vector>

namespace WordLearner
{
	class Database;

	// List widget for showing word sets from a given database.
	// Supports editing and deleting of word sets directly inside the list.
	class WordSetsListWidget : public QListWidget
	{
	public:
		// Creates an empty word sets list widget. Must be initialized by calling init() later.
		WordSetsListWidget(QWidget* parent = nullptr);

		// Initializes list widget with word sets from a given database
		void init(Database* database, std::function<void(const std::vector<Word>& words)> updateWordsWidgetCallback);

		// Updates word sets list widget to contain the given list of word sets. Removes all previously added word sets.
		// Keeps the currently selected word set selected after the update.
		// Optionally, you can provide ID of the word set to be selected.
		void update(const std::vector<WordSet>& wordSets, int selectedWordSetId = -1);

		// Returns ID of word set currently selected in UI
		int getSelectedWordSetId() const;

	private: /* functions*/

		// Creates all UI elements of widget
		void createUi();

	private slots:

		// Slot that's called when a new word set is selected in list widget
		void onSelectionChanged();

		// Slot that's called when a word set is edited in UI
		void onWordSetEdited(QListWidgetItem* item);

	private: /* variables */

		// List of IDs of word sets that are currently shown in list
		std::vector<int> m_wordSetsIds;

		// Callback function to update words widget with a given list of words
		std::function<void(const std::vector<Word>& words)> m_updateWordsWidgetCallback;

		Database* m_database;
	};

} // namespace WordLearner