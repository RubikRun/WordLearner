#pragma once

#include <QtWidgets/QMainWindow>

#include <QtWidgets/QListWidget>
#include <QtWidgets/QHBoxLayout>

#include "Database.h"

namespace WordLearner
{

    class WordLearnerMainWindow : public QMainWindow
    {
        Q_OBJECT

    public:
        // Creates a WordLearner main window with data from given database
        WordLearnerMainWindow(const Database& database, QWidget* parent = nullptr);
        ~WordLearnerMainWindow();

    private: /* functions */

        // Creates all UI elements of window
        void createUi();

        // Creates words list widget and fills it with words from database
        void createWordsListWidget();

    private: /* variables */

        // Struct for holding together window's UI elements
        struct UI
        {
            // Window's primary layout
            QHBoxLayout* layout = nullptr;
            // List widget for showing a list of words
            QListWidget* wordsListWidget = nullptr;
        } ui;

        const Database& database;
    };

} // namespace WordLearner