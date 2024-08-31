#include "WordLearnerMainWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    WordLearnerMainWindow w;
    w.show();
    return a.exec();
}
