#define WL_FILENAME "WordLearnerMainWindow.cpp"

#include "WordLearnerMainWindow.h"

#include "ResourceManager.h"
#include "Logger.hpp"
#include "CreateWordDialog.h"
#include "CreateWordSetDialog.h"

using namespace WordLearner;

WordLearnerMainWindow::WordLearnerMainWindow(Database& database, QWidget *parent)
    : QMainWindow(parent)
    , database(database)
{
    setWindowTitle("Word Learner");
    resize(800, 600);
    createUi();
}

WordLearnerMainWindow::~WordLearnerMainWindow()
{}

void WordLearnerMainWindow::onWordSetChanged()
{
    const int wordSetId = getSelectedWordSetId();
    if (wordSetId < 0)
    {
        return;
    }
    // Get words from selected word set
    const std::vector<Word> words = database.getWordsFromWordSet(wordSetId);
    // Update words list widget with words from set
    updateWordsListWidget(words);
}

void WordLearner::WordLearnerMainWindow::onCreateWordButtonPressed()
{
    // Create dialog for creating a new word
    CreateWordDialog createWordDialog(this);
    // Connect dialog's createWord() signal to our onCreateWord() slot here
    connect(&createWordDialog, &CreateWordDialog::createWord, this, &WordLearnerMainWindow::onCreateWord);
    // Open dialog
    createWordDialog.exec();
}

void WordLearner::WordLearnerMainWindow::onCreateWord(const std::string& termA, const std::string& termB, const std::string& note)
{
    // Create word in database
    const int wordId = database.createWord(termA, termB, note);
    if (wordId < 0)
    {
        WL_LOG_ERRORF("Failed to create new word in database.");
        return;
    }
    // Add new word to selected word set
    const int wordSetId = getSelectedWordSetId();
    if (wordSetId == 0)
    {
        // If selected word set is the global word set,
        // we don't need to explicitly add new word to it,
        // because it's automatically added when created.
        return;
    }
    if (wordSetId < 0)
    {
        WL_LOG_ERRORF("Trying to add new word to selected word set, but selected word set is invalid.");
        return;
    }
    if (!database.addWordToWordSet(wordId, wordSetId))
    {
        WL_LOG_ERRORF("Cannot add new word to selected word set.");
    }
    // Update words list in UI so that it shows the new word
    const std::vector<Word> words = database.getWordsFromWordSet(wordSetId);
    updateWordsListWidget(words);
}

void WordLearner::WordLearnerMainWindow::onCreateWordSetButtonPressed()
{
    // Create dialog for creating a new word set
    CreateWordSetDialog createWordSetDialog(this);
    // Connect dialog's createWordSet() signal to our onCreateWordSet() slot here
    connect(&createWordSetDialog, &CreateWordSetDialog::createWordSet, this, &WordLearnerMainWindow::onCreateWordSet);
    // Open dialog
    createWordSetDialog.exec();
}

void WordLearner::WordLearnerMainWindow::onCreateWordSet(const std::string& name)
{
    const int wordSetId = database.createWordSet(name);
    // Create word set in database
    if (wordSetId < 0)
    {
        WL_LOG_ERRORF("Failed to create new word set in database.");
        return;
    }
    // Update word sets list in UI so that it shows the new word set
    const std::vector<WordSet>& wordSets = database.getWordSets();
    updateWordSetsListWidget(wordSets, wordSetId);
}

void WordLearner::WordLearnerMainWindow::createUi()
{
    // Create central widget
    setCentralWidget(new QWidget);
    // Create layout
    ui.layout = new QHBoxLayout;
    centralWidget()->setLayout(ui.layout);
    // Create words layout and word sets layout
    ui.wordSetsLayout = new QVBoxLayout;
    ui.layout->addLayout(ui.wordSetsLayout);
    ui.wordsLayout = new QVBoxLayout;
    ui.layout->addLayout(ui.wordsLayout);
    // Create list widgets for words and word sets
    createWordSetsUi();
    createWordsUi();
}

void WordLearner::WordLearnerMainWindow::createWordSetsUi()
{
    // Create list widget
    ui.wordSetsListWidget = new QListWidget;
    ui.wordSetsListWidget->setStyleSheet(ResourceManager::getListWidgetStylesheet().c_str());
    ui.wordSetsLayout->addWidget(ui.wordSetsListWidget);
    // Connect word sets list widget's itemSelectionChanged() signal to our custom signal onWordSetChanged()
    connect(ui.wordSetsListWidget, &QListWidget::itemSelectionChanged, this, &WordLearnerMainWindow::onWordSetChanged);
    // Retrieve word sets list from database
    const std::vector<WordSet>& wordSets = database.getWordSets();
    // Fill list widget with word sets from database
    updateWordSetsListWidget(wordSets);
    // Create button for adding word sets
    ui.createWordSetButton = new QPushButton("New");
    ui.wordSetsLayout->addWidget(ui.createWordSetButton);
    connect(ui.createWordSetButton, &QPushButton::released, this, &WordLearnerMainWindow::onCreateWordSetButtonPressed);
}

void WordLearner::WordLearnerMainWindow::createWordsUi()
{
    // Create list widget
    ui.wordsListWidget = new QListWidget;
    ui.wordsListWidget->setStyleSheet(ResourceManager::getListWidgetStylesheet().c_str());
    ui.wordsLayout->addWidget(ui.wordsListWidget);
    // Retrieve words list from database
    const std::vector<Word>& words = database.getWords();
    // Fill list widget with words from database
    updateWordsListWidget(words);
    // Create button for adding words
    ui.createWordButton = new QPushButton("New");
    ui.wordsLayout->addWidget(ui.createWordButton);
    connect(ui.createWordButton, &QPushButton::released, this, &WordLearnerMainWindow::onCreateWordButtonPressed);
}

void WordLearner::WordLearnerMainWindow::updateWordsListWidget(const std::vector<Word>& words)
{
    // Remove all previously added words
    ui.wordsListWidget->clear();
    m_wordsListIds.clear();
    // Create a list of string items, one for each word
    QStringList items(words.size());
    for (int i = 0; i < words.size(); ++i)
    {
        const Word& word = words[i];
        const std::string wordView = word.termA + "  -  " + word.termB;
        items[i] = QString(wordView.c_str());
        // Add word ID to words IDs list
        m_wordsListIds.push_back(word.id);
    }
    // Add items to list widget
    ui.wordsListWidget->addItems(items);
}

void WordLearner::WordLearnerMainWindow::updateWordSetsListWidget(const std::vector<WordSet>& wordSets, int selectedWordSetId)
{
    // Remove all previously added word sets
    ui.wordSetsListWidget->clear();
    m_wordSetsListIds.clear();
    // Create a list of string items, one for each word set
    QStringList items(wordSets.size());
    int selectedWordSetIndex = -1;
    for (int i = 0; i < wordSets.size(); ++i)
    {
        const WordSet& wordSet = wordSets[i];
        const std::string wordSetView = (wordSet.id == 0) ? "*" : wordSet.name;
        items[i] = QString(wordSetView.c_str());
        // Add word set ID to word sets IDs list
        m_wordSetsListIds.push_back(wordSet.id);
        // Keep track of index of the word set that we want to be selected by default
        if (wordSet.id == selectedWordSetId)
        {
            selectedWordSetIndex = i;
        }
    }
    // Add items to list widget
    ui.wordSetsListWidget->addItems(items);
    // Select word set that should be selected by default
    if (selectedWordSetIndex >= 0)
    {
        ui.wordSetsListWidget->setCurrentRow(selectedWordSetIndex);
    }
}

int WordLearner::WordLearnerMainWindow::getSelectedWordSetId() const
{
    // Get index of selected word set in list
    const int wordSetIdx = ui.wordSetsListWidget->currentRow();
    // Get ID of selected word set
    if (wordSetIdx < 0 || wordSetIdx >= m_wordSetsListIds.size())
    {
        WL_LOG_ERRORF("Cannot get ID of selected word set, index out of range.");
        return -1;
    }
    return m_wordSetsListIds[wordSetIdx];
}
