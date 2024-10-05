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
    // Create list widget
    createWordsListWidget();
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
