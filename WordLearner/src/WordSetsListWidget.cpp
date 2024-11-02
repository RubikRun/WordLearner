#define WL_FILENAME "WordSetsListWidget.cpp"

#include "WordSetsListWidget.h"

#include "Database.h"
#include "ResourceManager.h"
#include "Logger.hpp"

using namespace WordLearner;

WordSetsListWidget::WordSetsListWidget(QWidget* parent)
    : QListWidget(parent)
{}

void WordLearner::WordSetsListWidget::init(Database* database, std::function<void(const std::vector<Word>& words)> updateWordsWidgetCallback)
{
    m_database = database;
    m_updateWordsWidgetCallback = updateWordsWidgetCallback;
    createUi();
}

void WordSetsListWidget::update(const std::vector<WordSet>& wordSets, int selectedWordSetId)
{
    // Temporarily disconnect itemChanged() signal
    // for the duration of this function, while we are adding new items,
    // because we don't want the slot to be called each time when we add a new item here,
    // we want it to be called only when user edits items in UI.
    disconnect(this, &QListWidget::itemChanged, this, &WordSetsListWidget::onWordSetEdited);

    // If no default selected word set ID is provided,
    // we would want the currently selected word set to remain selected after the update
    if (selectedWordSetId < 0)
    {
        selectedWordSetId = getSelectedWordSetId();
    }

    // Remove all previously added word sets
    clear();
    m_wordSetsIds.clear();
    // Fill list widget with items, one for each word set
    int selectedWordSetIndex = -1;
    for (int i = 0; i < wordSets.size(); ++i)
    {
        const WordSet& wordSet = wordSets[i];
        // Create an item from word set, and add it to list widget
        const std::string wordSetView = (wordSet.id == 0) ? "*" : wordSet.name;
        QListWidgetItem* item = new QListWidgetItem(QString(wordSetView.c_str()), this);
        // Make item be editable
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        // Add word set ID to word sets IDs list
        m_wordSetsIds.push_back(wordSet.id);
        // Keep track of index of the word set that we want to be selected by default
        if (wordSet.id == selectedWordSetId)
        {
            selectedWordSetIndex = i;
        }
    }
    // Select word set that should be selected by default
    if (selectedWordSetIndex >= 0)
    {
        setCurrentRow(selectedWordSetIndex);
    }

    // Connect word sets list widget's itemChanged() signal to our custom slot onWordSetEdited
    connect(this, &QListWidget::itemChanged, this, &WordSetsListWidget::onWordSetEdited);
}

int WordSetsListWidget::getSelectedWordSetId() const
{
    // Get index of selected word set in list
    const int wordSetIdx = currentRow();
    // Get ID of selected word set
    if (wordSetIdx < 0 || wordSetIdx >= m_wordSetsIds.size())
    {
        return -1;
    }
    return m_wordSetsIds[wordSetIdx];
}

void WordSetsListWidget::createUi()
{
    if (m_database == nullptr)
    {
        WL_LOG_ERRORF("Trying to create UI of word sets list widget but no database is given. Try properly initializing it.");
        return;
    }
    setStyleSheet(ResourceManager::getListWidgetStylesheet().c_str());
    // Connect word sets list widget's itemSelectionChanged() signal to our custom slot onWordSetSelectionChanged()
    connect(this, &QListWidget::itemSelectionChanged, this, &WordSetsListWidget::onSelectionChanged);
    // Retrieve word sets list from database
    const std::vector<WordSet>& wordSets = m_database->getWordSets();
    // Fill list widget with word sets from database
    update(wordSets);
}

void WordSetsListWidget::onSelectionChanged()
{
    if (m_database == nullptr)
    {
        WL_LOG_ERRORF("Selection in word sets list widget has changed but it's not yet initialized.");
        return;
    }
    const int wordSetId = getSelectedWordSetId();
    if (wordSetId < 0)
    {
        WL_LOG_ERRORF("Trying to show words of selected word set, but cannot find ID of that word set.");
        return;
    }

    // Update words widget with words from selected word set
    const std::vector<Word> words = m_database->getWordsFromWordSet(wordSetId);
    m_updateWordsWidgetCallback(words);
}

void WordSetsListWidget::onWordSetEdited(QListWidgetItem* item)
{
    if (m_database == nullptr)
    {
        WL_LOG_ERRORF("Trying to edit a word set but word sets list widget is not yet initialized.");
        return;
    }
    // Get index of edited word set
    const int wordSetIndex = row(item);
    // Get ID of edited word set
    if (wordSetIndex < 0 || wordSetIndex >= m_wordSetsIds.size())
    {
        WL_LOG_ERRORF("Cannot get ID of edited word set, row out of range.");
        return;
    }
    const int wordSetId = m_wordSetsIds[wordSetIndex];
    // Get new name for word set, entered by user
    const std::string newName = item->text().toStdString();
    // Edit word set's name in database
    if (!m_database->editWordSetName(wordSetId, newName))
    {
        WL_LOG_ERRORF("Failed to edit word set's name in database.");
    }
    // Update word sets list in UI.
    // When user edited word set's name in UI, that new name will be shown there without us doing anything,
    // but in case word set failed to update in database,
    // we would want to show the old name in UI, to indicate to user that the name they entered did not succeed.
    // In both cases, we just want UI to contain the current names of word sets from database, so just do update here.
    const std::vector<WordSet>& wordSets = m_database->getWordSets();
    update(wordSets);
}
