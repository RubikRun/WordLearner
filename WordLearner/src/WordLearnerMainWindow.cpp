#include "WordLearnerMainWindow.h"

#include "ResourceManager.h"

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

void WordLearner::WordLearnerMainWindow::createUi()
{
    // Create central widget
    setCentralWidget(new QWidget);
    // Create layout
    ui.layout = new QHBoxLayout;
    centralWidget()->setLayout(ui.layout);
    // Create list widgets for words and word sets
    createWordSetsListWidget();
    createWordsListWidget();
}

void WordLearner::WordLearnerMainWindow::createWordSetsListWidget()
{
    // Create list widget
    ui.wordSetsListWidget = new QListWidget;
    ui.wordSetsListWidget->setStyleSheet(ResourceManager::getListWidgetStylesheet().c_str());

    ui.layout->addWidget(ui.wordSetsListWidget);
    // Retrieve word sets list from database
    const std::vector<WordSet>& wordSets = database.getWordSets();
    // Create a list of string items, one for each word set
    QStringList items(wordSets.size());
    for (int i = 0; i < wordSets.size(); ++i)
    {
        const WordSet& wordSet = wordSets[i];
        const std::string wordSetView = wordSet.name;
        items[i] = QString(wordSetView.c_str());
    }
    // Add items to list widget
    ui.wordSetsListWidget->addItems(items);
}

void WordLearner::WordLearnerMainWindow::createWordsListWidget()
{
    // Create list widget
    ui.wordsListWidget = new QListWidget;
    ui.wordsListWidget->setStyleSheet(ResourceManager::getListWidgetStylesheet().c_str());

    ui.layout->addWidget(ui.wordsListWidget);
    // Retrieve words list from database
    const std::vector<Word>& words = database.getWords();
    // Create a list of string items, one for each word
    QStringList items(words.size());
    for (int i = 0; i < words.size(); ++i)
    {
        const Word& word = words[i];
        const std::string wordView = word.termA + "  -  " + word.termB;
        items[i] = QString(wordView.c_str());
    }
    // Add items to list widget
    ui.wordsListWidget->addItems(items);
}
