#pragma once

#include <QtWidgets/QMainWindow>

#include <QtWidgets/QListWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include "Database.h"

#include <vector>

namespace WordLearner
{
    class WordsWidget;

    // Main window of Word Learner application
    class WordLearnerMainWindow : public QMainWindow
    {
        Q_OBJECT

    public:

        // Creates a WordLearner main window with data from given database
        WordLearnerMainWindow(Database& database, QWidget* parent = nullptr);
        ~WordLearnerMainWindow();

    private slots:

        // Slot that's called when a new word set is selected in word sets list widget
        void onWordSetSelectionChanged();

        // Slot that's called when the "Create new word set" button is pressed
        void onCreateWordSetButtonPressed();
        // Slot that's called when a new word set must be created.
        // It's called with the new word set's data.
        void onCreateWordSet(const std::string& name);

        // Slot that's called when a word set is edited in UI
        void onWordSetEdited(QListWidgetItem* item);

    private: /* functions */

        // Creates all UI elements of window
        void createUi();

        // Creates the UI elements for word sets
        void createWordSetsUi();

        // Updates word sets list widget to contain the given list of word sets. Removes all previously added word sets.
        // Keeps the currently selected word set selected after the update.
        // Optionally, you can provide ID of the word set to be selected.
        void updateWordSetsListWidget(const std::vector<WordSet>& wordSets, int selectedWordSetId = -1);

        // Returns ID of word set currently selected in UI
        int getSelectedWordSetId() const;

    private: /* variables */

        // Struct for holding together window's UI elements
        struct Ui
        {
            // Window's primary layout
            QHBoxLayout* layout = nullptr;
            // Layout for word sets
            QVBoxLayout* wordSetsLayout = nullptr;
            // List widget for showing a list of word sets
            QListWidget* wordSetsListWidget = nullptr;
            // Button for creating a new word set
            QPushButton* createWordSetButton = nullptr;
            // Widget for showing, editing, creating and deleting words
            WordsWidget* wordsWidget = nullptr;
        } ui;

        // List of IDs of word sets that are currently shown in word sets list widget
        std::vector<int> m_wordSetsListIds;

        Database& m_database;
    };

} // namespace WordLearner