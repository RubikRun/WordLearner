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
		// Creates an empty words table widget. Must be initialized by calling init() later.
		WordsTableWidget(QWidget* parent = nullptr);

		// Initializes table widget with words from a given database
		void init(Database* database, std::function<int()> getSelectedWordSetIdCallback);

		// Updates words table widget to contain the given list of words. Removes all previously added words.
		void update(const std::vector<Word>& words);
		// Updates words table widget to contain the words of currently selected word set. Removes all previously added words.
		void update();

	private: /* functions*/

		// Creates all UI elements of widget
		void createUi();

	private slots:

		void onNoteEdited(int wordId, const std::string& newNote);

		// Slot that's called when a word is edited in UI
		void onWordEdited(int row, int col);

		// Slot that's called when a context menu must be shown at some item
		void showContextMenu(const QPoint& pos);

		// Slot that's called when user clicks "Edit note" option on a given word.
		// Opens up a small dialog where user can edit word's note.
		void onEditNoteOptionClicked(int wordId);

	private: /* variables */

		// List of IDs of words that are currently shown in table
		std::vector<int> m_wordsIds;

		// Callback function to retrieve ID of currently selected word set
		std::function<int()> m_getSelectedWordSetIdCallback;

		Database* m_database;
	};

} // namespace WordLearner