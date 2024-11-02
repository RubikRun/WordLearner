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
    class WordSetsListWidget;

    // Main window of Word Learner application
    class WordLearnerMainWindow : public QMainWindow
    {
        Q_OBJECT

    public:

        // Creates a WordLearner main window with data from given database
        WordLearnerMainWindow(Database& database, QWidget* parent = nullptr);
        ~WordLearnerMainWindow();

    private slots:

        // Slot that's called when the "Create new word set" button is pressed
        void onCreateWordSetButtonPressed();
        // Slot that's called when a new word set must be created.
        // It's called with the new word set's data.
        void onCreateWordSet(const std::string& name);

    private: /* functions */

        // Creates all UI elements of window
        void createUi();

    private: /* variables */

        // Struct for holding together window's UI elements
        struct Ui
        {
            // Window's primary layout
            QHBoxLayout* layout = nullptr;
            // Layout for word sets
            QVBoxLayout* wordSetsLayout = nullptr;
            // List widget for showing a list of word sets
            WordSetsListWidget* wordSetsListWidget = nullptr;
            // Button for creating a new word set
            QPushButton* createWordSetButton = nullptr;
            // Widget for showing, editing, creating and deleting words
            WordsWidget* wordsWidget = nullptr;
        } ui;

        Database& m_database;
    };

} // namespace WordLearner