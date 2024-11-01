#define WL_FILENAME "WordLearnerMainWindow.cpp"

#include "WordLearnerMainWindow.h"

#include "WordsWidget.h"
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

void WordLearnerMainWindow::onWordSetSelectionChanged()
{
    const int wordSetId = getSelectedWordSetId();
    if (wordSetId < 0)
    {
        WL_LOG_ERRORF("Trying to show words of selected word set, but cannot find ID of that word set.");
        return;
    }
    // Get words from selected word set
    const std::vector<Word> words = m_database.getWordsFromWordSet(wordSetId);
    // Update words widget with words from set
    ui.wordsWidget->update(words);
}

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
    updateWordSetsListWidget(wordSets, wordSetId);
}

void WordLearnerMainWindow::onWordSetEdited(QListWidgetItem* item)
{
    // Get index of edited word set
    const int wordSetIndex = ui.wordSetsListWidget->row(item);
    // Get ID of edited word set
    if (wordSetIndex < 0 || wordSetIndex >= m_wordSetsListIds.size())
    {
        WL_LOG_ERRORF("Cannot get ID of edited word set, row out of range.");
        return;
    }
    const int wordSetId = m_wordSetsListIds[wordSetIndex];
    // Get new name for word set, entered by user
    const std::string newName = item->text().toStdString();
    // Edit word set's name in database
    if (!m_database.editWordSetName(wordSetId, newName))
    {
        WL_LOG_ERRORF("Failed to edit word set's name in database.");
    }
    // Update word sets list in UI.
    // When user edited word set's name in UI, that new name will be shown there without us doing anything,
    // but in case word set failed to update in database,
    // we would want to show the old name in UI, to indicate to user that the name they entered did not succeed.
    // In both cases, we just want UI to contain the current names of word sets from database, so just do update here.
    const std::vector<WordSet>& wordSets = m_database.getWordSets();
    updateWordSetsListWidget(wordSets);
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
    // Create list widgets for word sets
    createWordSetsUi();

    // Create words widget
    ui.wordsWidget = new WordsWidget(
        m_database,
        std::bind(&WordLearnerMainWindow::getSelectedWordSetId, this),
        this
    );
    ui.layout->addWidget(ui.wordsWidget);
}

void WordLearnerMainWindow::createWordSetsUi()
{
    // Create list widget
    ui.wordSetsListWidget = new QListWidget;
    ui.wordSetsListWidget->setStyleSheet(ResourceManager::getListWidgetStylesheet().c_str());
    ui.wordSetsLayout->addWidget(ui.wordSetsListWidget);
    // Connect word sets list widget's itemSelectionChanged() signal to our custom slot onWordSetSelectionChanged()
    connect(ui.wordSetsListWidget, &QListWidget::itemSelectionChanged, this, &WordLearnerMainWindow::onWordSetSelectionChanged);
    // Retrieve word sets list from database
    const std::vector<WordSet>& wordSets = m_database.getWordSets();
    // Fill list widget with word sets from database
    updateWordSetsListWidget(wordSets);
    // Create button for adding word sets
    ui.createWordSetButton = new QPushButton("New");
    ui.wordSetsLayout->addWidget(ui.createWordSetButton);
    connect(ui.createWordSetButton, &QPushButton::released, this, &WordLearnerMainWindow::onCreateWordSetButtonPressed);
}

void WordLearnerMainWindow::updateWordSetsListWidget(const std::vector<WordSet>& wordSets, int selectedWordSetId)
{
    // Temporarily disconnect word sets list widget's itemChanged() signal
    // for the duration of this function, while we are adding new items,
    // because we don't want the slot to be called each time when we add a new item here,
    // we want it to be called only when user edits items in UI.
    disconnect(ui.wordSetsListWidget, &QListWidget::itemChanged, this, &WordLearnerMainWindow::onWordSetEdited);

    // If no default selected word set ID is provided,
    // we would want the currently selected word set to remain selected after the update
    if (selectedWordSetId < 0)
    {
        selectedWordSetId = getSelectedWordSetId();
    }

    // Remove all previously added word sets
    ui.wordSetsListWidget->clear();
    m_wordSetsListIds.clear();
    // Fill list widget with items, one for each word set
    int selectedWordSetIndex = -1;
    for (int i = 0; i < wordSets.size(); ++i)
    {
        const WordSet& wordSet = wordSets[i];
        // Create an item from word set, and add it to list widget
        const std::string wordSetView = (wordSet.id == 0) ? "*" : wordSet.name;
        QListWidgetItem* item = new QListWidgetItem(QString(wordSetView.c_str()), ui.wordSetsListWidget);
        // Make item be editable
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        // Add word set ID to word sets IDs list
        m_wordSetsListIds.push_back(wordSet.id);
        // Keep track of index of the word set that we want to be selected by default
        if (wordSet.id == selectedWordSetId)
        {
            selectedWordSetIndex = i;
        }
    }
    // Select word set that should be selected by default
    if (selectedWordSetIndex >= 0)
    {
        ui.wordSetsListWidget->setCurrentRow(selectedWordSetIndex);
    }

    // Connect word sets list widget's itemChanged() signal to our custom slot onWordSetEdited
    connect(ui.wordSetsListWidget, &QListWidget::itemChanged, this, &WordLearnerMainWindow::onWordSetEdited);
}

int WordLearnerMainWindow::getSelectedWordSetId() const
{
    // Get index of selected word set in list
    const int wordSetIdx = ui.wordSetsListWidget->currentRow();
    // Get ID of selected word set
    if (wordSetIdx < 0 || wordSetIdx >= m_wordSetsListIds.size())
    {
        return -1;
    }
    return m_wordSetsListIds[wordSetIdx];
}
