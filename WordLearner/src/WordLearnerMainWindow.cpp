#define WL_FILENAME "WordLearnerMainWindow.cpp"

#include "WordLearnerMainWindow.h"

#include "ResourceManager.h"
#include "Logger.hpp"
#include "CreateWordDialog.h"

using namespace WordLearner;

WordLearnerMainWindow::WordLearnerMainWindow(const Database& database, QWidget *parent)
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
    // Get index of selected word set in list
    const int wordSetIdx = ui.wordSetsListWidget->currentRow();
    // Get ID of selected word set
    if (wordSetIdx < 0 || wordSetIdx >= wordSetsListIds.size())
    {
        WL_LOG_ERRORF("Cannot get ID of selected word set, index out of range.");
        return;
    }
    const int wordSetId = wordSetsListIds[wordSetIdx];
    // Get words from selected word set
    const std::vector<Word> words = database.getWordsFromWordSet(wordSetId);
    // Update words list widget with words from set
    updateWordsListWidget(words);
}

void WordLearner::WordLearnerMainWindow::onCreateWordButtonPressed()
{
    // Create dialog for creating of a new word
    CreateWordDialog createWordDialog(this);
    // Connect dialog's createWord() signal to our onCreateWord() slot here
    connect(&createWordDialog, &CreateWordDialog::createWord, this, &WordLearnerMainWindow::onCreateWord);
    // Open dialog
    createWordDialog.exec();
}

void WordLearner::WordLearnerMainWindow::onCreateWord(const std::string& termA, const std::string& termB, const std::string& note)
{
    qDebug() << "onCreateWordDialogFinished() <----------";
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
    // Create a list of string items, one for each word set
    QStringList items(wordSets.size());
    for (int i = 0; i < wordSets.size(); ++i)
    {
        const WordSet& wordSet = wordSets[i];
        const std::string wordSetView = (wordSet.id == 0) ? "*" : wordSet.name;
        items[i] = QString(wordSetView.c_str());
        // Add word set ID to word sets IDs list
        wordSetsListIds.push_back(wordSet.id);
    }
    // Add items to list widget
    ui.wordSetsListWidget->addItems(items);
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
    // Create a list of string items, one for each word
    QStringList items(words.size());
    for (int i = 0; i < words.size(); ++i)
    {
        const Word& word = words[i];
        const std::string wordView = word.termA + "  -  " + word.termB;
        items[i] = QString(wordView.c_str());
        // Add word ID to words IDs list
        wordsListIds.push_back(word.id);
    }
    // Add items to list widget
    ui.wordsListWidget->addItems(items);
}
