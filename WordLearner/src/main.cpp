#include "WordLearnerMainWindow.h"
#include <QtWidgets/QApplication>

// TEMP: Testing to see if Database links properly
#include "Database.h"

int main(int argc, char *argv[])
{
    // TEMP: Testing to see if Database links properly
    WordLearner::Database::databaseFunction();

    QApplication a(argc, argv);
    WordLearnerMainWindow w;
    w.show();
    return a.exec();
}
