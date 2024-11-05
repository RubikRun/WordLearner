#define WL_FILENAME "WordSetsWidget.cpp"

#include "WordSetsWidget.h"

#include "WordSetsListWidget.h"
#include "Database.h"
#include "ResourceManager.h"
#include "Logger.hpp"
#include "CreateWordSetDialog.h"

using namespace WordLearner;

WordSetsWidget::WordSetsWidget(QWidget* parent)
    : QWidget(parent)
{}

void WordSetsWidget::init(Database* database, std::function<void(const std::vector<Word>& words)> updateWordsWidgetCallback)
{
    m_database = database;
    m_updateWordsWidgetCallback = updateWordsWidgetCallback;
    createUi();
}

void WordSetsWidget::update(const std::vector<WordSet>& wordSets, int selectedWordSetId)
{
    ui.wordSetsListWidget->update(wordSets);
}

int WordLearner::WordSetsWidget::getSelectedWordSetId() const
{
    if (ui.wordSetsListWidget == nullptr)
    {
        WL_LOG_ERRORF("Trying to get selected word set but word sets list widget is not yet created. Try properly initializing word sets widget.");
        return -1;
    }
    return ui.wordSetsListWidget->getSelectedWordSetId();
}

void WordSetsWidget::createUi()
{
    if (m_database == nullptr)
    {
        WL_LOG_ERRORF("Trying to create UI of word sets widget but no database is given. Try properly initializing it.");
        return;
    }
    // Create layout
    ui.layout = new QVBoxLayout;
    setLayout(ui.layout);

    // Create word sets list widget
    ui.wordSetsListWidget = new WordSetsListWidget(this);
    ui.wordSetsListWidget->init(m_database, m_updateWordsWidgetCallback);
    ui.layout->addWidget(ui.wordSetsListWidget);
    // Create button for adding word sets
    ui.createWordSetButton = new QPushButton("New");
    ui.layout->addWidget(ui.createWordSetButton);
    connect(ui.createWordSetButton, &QPushButton::released, this, &WordSetsWidget::onCreateWordSetButtonPressed);
}

void WordSetsWidget::onCreateWordSetButtonPressed()
{
    // Create dialog for creating a new word set
    CreateWordSetDialog createWordSetDialog(this);
    // Connect dialog's createWordSet() signal to our onCreateWordSet() slot here
    connect(&createWordSetDialog, &CreateWordSetDialog::createWordSet, this, &WordSetsWidget::onCreateWordSet);
    // Open dialog
    createWordSetDialog.exec();
}

void WordSetsWidget::onCreateWordSet(const std::string& name)
{
    const int wordSetId = m_database->createWordSet(name);
    // Create word set in database
    if (wordSetId < 0)
    {
        WL_LOG_ERRORF("Failed to create new word set in database.");
        return;
    }
    // Update word sets list in UI so that it shows the new word set
    const std::vector<WordSet>& wordSets = m_database->getWordSets();
    ui.wordSetsListWidget->update(wordSets, wordSetId);
}