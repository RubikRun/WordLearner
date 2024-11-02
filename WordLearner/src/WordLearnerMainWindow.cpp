#define WL_FILENAME "WordLearnerMainWindow.cpp"

#include "WordLearnerMainWindow.h"

#include "WordsWidget.h"
#include "WordSetsListWidget.h"
#include "ResourceManager.h"
#include "Logger.hpp"
#include "CreateWordSetDialog.h"

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

void WordLearnerMainWindow::onCreateWordSetButtonPressed()
{
    // Create dialog for creating a new word set
    CreateWordSetDialog createWordSetDialog(this);
    // Connect dialog's createWordSet() signal to our onCreateWordSet() slot here
    connect(&createWordSetDialog, &CreateWordSetDialog::createWordSet, this, &WordLearnerMainWindow::onCreateWordSet);
    // Open dialog
    createWordSetDialog.exec();
}

void WordLearnerMainWindow::onCreateWordSet(const std::string& name)
{
    const int wordSetId = m_database.createWordSet(name);
    // Create word set in database
    if (wordSetId < 0)
    {
        WL_LOG_ERRORF("Failed to create new word set in database.");
        return;
    }
    // Update word sets list in UI so that it shows the new word set
    const std::vector<WordSet>& wordSets = m_database.getWordSets();
    ui.wordSetsListWidget->update(wordSets, wordSetId);
}

void WordLearnerMainWindow::createUi()
{
    // Create central widget
    setCentralWidget(new QWidget);
    // Create layout
    ui.layout = new QHBoxLayout;
    centralWidget()->setLayout(ui.layout);

    // Create word sets layout
    ui.wordSetsLayout = new QVBoxLayout;
    ui.layout->addLayout(ui.wordSetsLayout);

    // First create both word sets list widget and words widget,
    // without initializing them,
    // because their initializations depend on each other.
    ui.wordSetsListWidget = new WordSetsListWidget(this);
    ui.wordSetsLayout->addWidget(ui.wordSetsListWidget);
    ui.wordsWidget = new WordsWidget(this);
    ui.layout->addWidget(ui.wordsWidget);
    // Then initialize them
    WordsWidget* wordsWidget = ui.wordsWidget;
    ui.wordSetsListWidget->init(
        &m_database,
        // NOTE: std::bind() doesn't work here for some reason, so use lambda instead
        [wordsWidget](const std::vector<Word>& words) {
            wordsWidget->update(words);
        }
    );
    ui.wordsWidget->init(
        &m_database,
        std::bind(&WordSetsListWidget::getSelectedWordSetId, ui.wordSetsListWidget)
    );

    // Create button for adding word sets
    ui.createWordSetButton = new QPushButton("New");
    ui.wordSetsLayout->addWidget(ui.createWordSetButton);
    connect(ui.createWordSetButton, &QPushButton::released, this, &WordLearnerMainWindow::onCreateWordSetButtonPressed);
}
