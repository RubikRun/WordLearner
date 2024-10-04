#include <windows.h>

#include "WordLearnerMainWindow.h"
#include <QtWidgets/QApplication>

#include "Database.h"

using WordLearner::Database;
using WordLearner::WordLearnerMainWindow;

int main(int argc, char *argv[])
{
    // Set console's output code page to UTF-8
    // so that it can write cyrillic characters
    SetConsoleOutputCP(CP_UTF8);
    // Load database
    Database database;
    database.loadDatabase();
    // Create Qt application showing main window and run it
    QApplication application(argc, argv);
    WordLearnerMainWindow window(database);
    window.show();
    const int execResult = application.exec();
    // Export database after application has closed
    database.exportDatabase();
    return execResult;
}