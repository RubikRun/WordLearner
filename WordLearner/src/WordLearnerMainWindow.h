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
    class WordSetsWidget;

    // Main window of Word Learner application
    class WordLearnerMainWindow : public QMainWindow
    {
        Q_OBJECT

    public:

        // Creates a WordLearner main window with data from given database
        WordLearnerMainWindow(Database& database, QWidget* parent = nullptr);
        ~WordLearnerMainWindow();

    private: /* functions */

        // Creates all UI elements of window
        void createUi();

    private: /* variables */

        // Struct for holding together window's UI elements
        struct Ui
        {
            // Window's primary layout
            QHBoxLayout* layout = nullptr;
            // Widget for showing, editing, creating and deleting word sets
            WordSetsWidget* wordSetsWidget = nullptr;
            // Widget for showing, editing, creating and deleting words
            WordsWidget* wordsWidget = nullptr;
        } ui;

        Database& m_database;
    };

} // namespace WordLearner