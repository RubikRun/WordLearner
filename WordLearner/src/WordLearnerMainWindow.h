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
        WordLearnerMainWindow(const Database& database, QWidget* parent = nullptr);
        ~WordLearnerMainWindow();

    private slots:

        // Slot that's called when a new word set is selected in word sets list widget
        void onWordSetChanged();

    private: /* functions */

        // Creates all UI elements of window
        void createUi();

        // Creates the UI elements for word sets
        void createWordSetsUi();
        // Creates the UI elements for words
        void createWordsUi();

        // Updates words list widget to contain the given list of words. Removes all previously added words.
        void updateWordsListWidget(const std::vector<Word>& words);

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
            // Layout for words
            QVBoxLayout* wordsLayout = nullptr;
            // List widget for showing a list of words
            QListWidget* wordsListWidget = nullptr;
            // Button for adding a new word
            QPushButton* addWordButton = nullptr;
        } ui;

        // List of IDs of words that are currently shown in words list widget
        std::vector<int> wordsListIds;
        // List of IDs of word sets that are currently shown in word sets list widget
        std::vector<int> wordSetsListIds;

        const Database& database;
    };

} // namespace WordLearner