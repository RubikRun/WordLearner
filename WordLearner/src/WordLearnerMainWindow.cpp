#include "WordLearnerMainWindow.h"

WordLearnerMainWindow::WordLearnerMainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setWindowTitle("Word Learner");
    ui.mainToolBar->hide();
}

WordLearnerMainWindow::~WordLearnerMainWindow()
{}
