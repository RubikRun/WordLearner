#include <windows.h>

#include "WordLearnerMainWindow.h"
#include <QtWidgets/QApplication>

#include "Database.h"

int main(int argc, char *argv[])
{
    // Set console's output code page to UTF-8
    // so that it can write cyrillic characters
    SetConsoleOutputCP(CP_UTF8);
    // Load and export database
    WordLearner::Database database;
    database.loadDatabase();
    database.exportDatabase();
    // Create Qt application and run it
    QApplication a(argc, argv);
    WordLearnerMainWindow w;
    w.show();
    return a.exec();
}