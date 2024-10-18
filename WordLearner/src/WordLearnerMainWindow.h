#pragma once

#include <QtWidgets/QMainWindow>

#include <QtWidgets/QListWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QPushButton>

#include "Database.h"

#include <vector>

namespace WordLearner
{

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

        // Slot that's called when the "Create new word" button is pressed
        void onCreateWordButtonPressed();
        // Slot that's called when a new word must be created.
        // It's called with the new word's data.
        void onCreateWord(const std::string& termA, const std::string& termB, const std::string& note);

        // Slot that's called when the "Create new word set" button is pressed
        void onCreateWordSetButtonPressed();
        // Slot that's called when a new word set must be created.
        // It's called with the new word set's data.
        void onCreateWordSet(const std::string& name);

        // Slot that's called when a word set is edited in UI.
        void onWordSetEdited(QListWidgetItem* item);

    private: /* functions */

        // Creates all UI elements of window
        void createUi();

        // Creates the UI elements for word sets
        void createWordSetsUi();
        // Creates the UI elements for words
        void createWordsUi();

        // Updates words list widget to contain the given list of words. Removes all previously added words.
        void updateWordsListWidget(const std::vector<Word>& words);
        // Updates word sets list widget to contain the given list of word sets. Removes all previously added word sets.
        // Optionally, you can provide ID of the word set to be selected by default, at the beginning before any user interaction.
        void updateWordSetsListWidget(const std::vector<WordSet>& wordSets, int selectedWordSetId = -1);

        // Returns ID of word set currently selected in UI
        int getSelectedWordSetId() const;

    private: /* variables */

        // Struct for holding together window's UI elements
        struct UI
        {
            // Window's primary layout
            QHBoxLayout* layout = nullptr;
            // Layout for word sets
            QVBoxLayout* wordSetsLayout = nullptr;
            // List widget for showing a list of word sets
            QListWidget* wordSetsListWidget = nullptr;
            // Button for creating a new word set
            QPushButton* createWordSetButton = nullptr;
            // Layout for words
            QVBoxLayout* wordsLayout = nullptr;
            // List widget for showing a list of words
            QListWidget* wordsListWidget = nullptr;
            // Button for creating a new word
            QPushButton* createWordButton = nullptr;
        } ui;

        // List of IDs of words that are currently shown in words list widget
        std::vector<int> m_wordsListIds;
        // List of IDs of word sets that are currently shown in word sets list widget
        std::vector<int> m_wordSetsListIds;

        Database& database;
    };

} // namespace WordLearner