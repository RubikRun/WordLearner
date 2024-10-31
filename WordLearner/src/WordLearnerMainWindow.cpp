#define WL_FILENAME "WordLearnerMainWindow.cpp"

#include "WordLearnerMainWindow.h"

#include "ResourceManager.h"
#include "Logger.hpp"
#include "StringUtils.h"
#include "CreateWordDialog.h"
#include "CreateWordSetDialog.h"

#include <QHeaderView>

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

void WordLearnerMainWindow::onWordSetSelectionChanged()
{
    const int wordSetId = getSelectedWordSetId();
    if (wordSetId < 0)
    {
        WL_LOG_ERRORF("Trying to show words of selected word set, but cannot find ID of that word set.");
        return;
    }
    // Get words from selected word set
    const std::vector<Word> words = database.getWordsFromWordSet(wordSetId);
    // Update words list widget with words from set
    updateWordsTableWidget(words);
}

void WordLearner::WordLearnerMainWindow::onCreateWordButtonPressed()
{
    // Create dialog for creating a new word
    CreateWordDialog createWordDialog(database.getLanguageA(), database.getLanguageB(), this);
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
    // Get selected word set's ID
    const int wordSetId = getSelectedWordSetId();
    if (wordSetId < 0)
    {
        WL_LOG_ERRORF("Trying to add new word to selected word set, but there is no selected word set.");
        return;
    }
    // Add new word to selected word set.
    // If selected word set is the global word set,
    // we don't need to explicitly add new word to it,
    // because it's automatically added when created.
    if (wordSetId != 0 && !database.addWordToWordSet(wordId, wordSetId))
    {
        WL_LOG_ERRORF("Cannot add new word to selected word set.");
    }
    // Update words list in UI so that it shows the new word
    const std::vector<Word> words = database.getWordsFromWordSet(wordSetId);
    updateWordsTableWidget(words);
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

void WordLearner::WordLearnerMainWindow::onWordSetEdited(QListWidgetItem* item)
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
    if (!database.editWordSetName(wordSetId, newName))
    {
        WL_LOG_ERRORF("Failed to edit word set's name in database.");
    }
    // Update word sets list in UI.
    // When user edited word set's name in UI, that new name will be shown there without us doing anything,
    // but in case word set failed to update in database,
    // we would want to show the old name in UI, to indicate to user that the name they entered did not succeed.
    // In both cases, we just want UI to contain the current names of word sets from database, so just do update here.
    const std::vector<WordSet>& wordSets = database.getWordSets();
    updateWordSetsListWidget(wordSets);
}

void WordLearner::WordLearnerMainWindow::onWordEdited(int row, int col)
{
    QTableWidgetItem* tableItem = ui.wordsTableWidget->item(row, col);
    if (!tableItem)
    {
        WL_LOG_ERRORF("User edited an invalid cell of the words table, out of bounds.");
        return;
    }
    // Get the new string value that user has entered
    const std::string editedStr = tableItem->text().toStdString();
    // Get ID of edited word
    if (row < 0 || row >= m_wordsListIds.size())
    {
        WL_LOG_ERRORF("Cannot get ID of edited word, row out of range.");
        return;
    }
    const int wordId = m_wordsListIds[row];
    // If user has edited word's term A
    if (col == 0)
    {
        // Edit word's term A in database
        if (!database.editWordTermA(wordId, editedStr))
        {
            WL_LOG_ERRORF("Failed to edit word's term A in database.");
        }
    }
    // If user has edited word's term B
    else if (col == 1)
    {
        // Edit word's term B in database

        if (!database.editWordTermB(wordId, editedStr))
        {
            WL_LOG_ERRORF("Failed to edit word's term B in database.");
        }
    }
    else
    {
        WL_LOG_ERRORF("User edited an invalid cell of the words table, column out of bounds.");
        return;
    }
    // Update words table in UI with the words of the currently selected word set.
    // When user edited some word's property in UI, that new property will be shown there without us doing anything,
    // but in case word failed to update in database,
    // we would want to show the old property in UI, to indicate to user that the property they entered did not succeed.
    // In both cases, we just want UI to contain the current properties of words from database, so just do update here.
    const int selectedWordSetId = getSelectedWordSetId();
    const std::vector<Word>& words = database.getWordsFromWordSet(selectedWordSetId);
    updateWordsTableWidget(words);
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
    // Connect word sets list widget's itemSelectionChanged() signal to our custom slot onWordSetSelectionChanged()
    connect(ui.wordSetsListWidget, &QListWidget::itemSelectionChanged, this, &WordLearnerMainWindow::onWordSetSelectionChanged);
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
    // Create table widgeet
    ui.wordsTableWidget = new QTableWidget;
    ui.wordsTableWidget->setColumnCount(2);
    ui.wordsTableWidget->setStyleSheet(ResourceManager::getTableWidgetStylesheet().c_str());
    ui.wordsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.wordsTableWidget->setHorizontalHeaderLabels({
        QString(database.getLanguageA().c_str()),
        QString(database.getLanguageB().c_str())
    });
    ui.wordsLayout->addWidget(ui.wordsTableWidget);
    // Retrieve words list from database
    const std::vector<Word>& words = database.getWords();
    // Fill table widget with words from database
    updateWordsTableWidget(words);
    // Create button for adding words
    ui.createWordButton = new QPushButton("New");
    ui.wordsLayout->addWidget(ui.createWordButton);
    connect(ui.createWordButton, &QPushButton::released, this, &WordLearnerMainWindow::onCreateWordButtonPressed);
}

void WordLearner::WordLearnerMainWindow::updateWordsTableWidget(const std::vector<Word>& words)
{
    // Temporarily disconnect words table widget's cellChanged() signal
    // for the duration of this function, while we are filling cells,
    // because we don't want the slot to be called each time when we fill a cell here,
    // we want it to be called only when user edits cells in UI.
    disconnect(ui.wordsTableWidget, &QTableWidget::cellChanged, this, &WordLearnerMainWindow::onWordEdited);

    // Remove all previously added words
    ui.wordsTableWidget->clearContents();
    m_wordsListIds.clear();
    // Fill table widget with rows, one for each word
    ui.wordsTableWidget->setRowCount(int(words.size()));
    for (int i = 0; i < words.size(); ++i)
    {
        const Word& word = words[i];
        // Set columns of table's current row to be the word's term A and term B
        QTableWidgetItem* itemTermA = new QTableWidgetItem(QString(word.termA.c_str()));
        QTableWidgetItem* itemTermB = new QTableWidgetItem(QString(word.termB.c_str()));
        // Set a tooltip that shows word's note. Set it to both items (to the whole row)
        itemTermA->setToolTip(StringUtils::breakIntoMultipleLines(word.note).c_str());
        itemTermB->setToolTip(StringUtils::breakIntoMultipleLines(word.note).c_str());
        // Add items to words table
        ui.wordsTableWidget->setItem(i, 0, itemTermA);
        ui.wordsTableWidget->setItem(i, 1, itemTermB);
        // Add word ID to words IDs list
        m_wordsListIds.push_back(word.id);
    }

    // Connect words table widget's cellChanged() signal to our custom slot onWordEdited
    connect(ui.wordsTableWidget, &QTableWidget::cellChanged, this, &WordLearnerMainWindow::onWordEdited);
}

void WordLearner::WordLearnerMainWindow::updateWordSetsListWidget(const std::vector<WordSet>& wordSets, int selectedWordSetId)
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

int WordLearner::WordLearnerMainWindow::getSelectedWordSetId() const
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
