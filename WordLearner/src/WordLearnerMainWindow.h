#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_WordLearnerMainWindow.h"

class WordLearnerMainWindow : public QMainWindow
{
    Q_OBJECT

public:
    WordLearnerMainWindow(QWidget *parent = nullptr);
    ~WordLearnerMainWindow();

private:
    Ui::WordLearnerMainWindowClass ui;
};
