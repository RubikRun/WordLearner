#pragma once

#include "Word.h"

#include <QtWidgets/QTableWidget>
#include <vector>
#include <functional>

namespace WordLearner
{
	class Database;

	// Table widget for showing words from a given database.
	// Supports editing and deleting of words directly inside the table.
	class WordsTableWidget : public QTableWidget
	{
	public:
		// Creates a table widget for the words of a given database
		WordsTableWidget(Database& database, std::function<int()> getSelectedWordSetIdCallback, QWidget* parent = nullptr);

		// Updates words table widget to contain the given list of words. Removes all previously added words.
		void update(const std::vector<Word>& words);

	private slots:
		// Slot that's called when a word is edited in UI
		void onWordEdited(int row, int col);

	private: /* variables */
		// List of IDs of words that are currently shown in table
		std::vector<int> m_wordsIds;

		// Callback function to retrieve ID of currently selected word set
		std::function<int()> m_getSelectedWordSetIdCallback;

		Database& m_database;
	};

} // namespace WordLearner