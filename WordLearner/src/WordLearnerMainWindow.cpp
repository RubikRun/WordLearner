#define WL_FILENAME "WordLearnerMainWindow.cpp"

#include "WordLearnerMainWindow.h"

#include "WordsWidget.h"
#include "WordSetsWidget.h"
#include "ResourceManager.h"
#include "Logger.hpp"

using namespace WordLearner;

WordLearnerMainWindow::WordLearnerMainWindow(Database& database, QWidget *parent)
    : QMainWindow(parent)
    , m_database(database)
{
    setWindowTitle("Word Learner");
    resize(800, 600);
    createUi();
}

WordLearnerMainWindow::~WordLearnerMainWindow()
{}

void WordLearnerMainWindow::createUi()
{
    // Create central widget
    setCentralWidget(new QWidget);
    // Create layout
    ui.layout = new QHBoxLayout;
    centralWidget()->setLayout(ui.layout);

    // First create both word sets widget and words widget,
    // without initializing them,
    // because their initializations depend on each other.
    ui.wordSetsWidget = new WordSetsWidget(this);
    ui.layout ->addWidget(ui.wordSetsWidget);
    ui.wordsWidget = new WordsWidget(this);
    ui.layout->addWidget(ui.wordsWidget);
    // Then initialize them
    WordsWidget* wordsWidget = ui.wordsWidget;
    ui.wordSetsWidget->init(
        &m_database,
        // NOTE: std::bind() doesn't work here for some reason, so use lambda instead
        [wordsWidget](const std::vector<Word>& words) {
            wordsWidget->update(words);
        }
    );
    ui.wordsWidget->init(
        &m_database,
        std::bind(&WordSetsWidget::getSelectedWordSetId, ui.wordSetsWidget)
    );
}
