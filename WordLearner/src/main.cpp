#include <windows.h>

#include "WordLearnerMainWindow.h"
#include "Database.h"
#include "ResourceManager.h"

#include <QtWidgets/QApplication>


using namespace WordLearner;

static void setup()
{
    // Set console's output code page to UTF-8
    // so that it can write cyrillic characters
    SetConsoleOutputCP(CP_UTF8);
    // Load global resources
    ResourceManager::load();
}

int main(int argc, char *argv[])
{
    setup();
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