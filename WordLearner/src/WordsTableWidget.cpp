#define WL_FILENAME "WordsTableWidget.cpp"

#include "WordsTableWidget.h"

#include "Database.h"
#include "ResourceManager.h"
#include "Logger.hpp"
#include "StringUtils.h"

#include <QHeaderView>

using namespace WordLearner;

WordsTableWidget::WordsTableWidget(Database& database, std::function<int()> getSelectedWordSetIdCallback, QWidget* parent)
	: QTableWidget(parent)
    , m_database(database)
    , m_getSelectedWordSetIdCallback(getSelectedWordSetIdCallback)
{
    createUi();
}

void WordsTableWidget::update(const std::vector<Word>& words)
{
    // Temporarily disconnect cellChanged() signal
    // for the duration of this function, while we are filling items,
    // because we don't want the slot to be called each time when we fill an ietm here,
    // we want it to be called only when user edits items in UI.
    disconnect(this, &QTableWidget::cellChanged, this, &WordsTableWidget::onWordEdited);

    // Remove all previously added words
    clearContents();
    m_wordsIds.clear();
    // Fill table widget with rows, one for each word
    setRowCount(int(words.size()));
    for (int i = 0; i < words.size(); ++i)
    {
        const Word& word = words[i];
        // Set columns of table's current row to be the word's term A and term B
        const std::string termAView = word.note.empty() ? word.termA : word.termA + "*";
        const std::string termBView = word.termB;
        QTableWidgetItem* itemTermA = new QTableWidgetItem(QString(termAView.c_str()));
        QTableWidgetItem* itemTermB = new QTableWidgetItem(QString(termBView.c_str()));
        // If word has a note
        if (!word.note.empty())
        {
            // Set a tooltip that shows word's note. Set it to both items (to the whole row)
            itemTermA->setToolTip(StringUtils::breakIntoMultipleLines(word.note).c_str());
            itemTermB->setToolTip(StringUtils::breakIntoMultipleLines(word.note).c_str());
        }
        // Add items to words table
        setItem(i, 0, itemTermA);
        setItem(i, 1, itemTermB);
        // Add word ID to words IDs list
        m_wordsIds.push_back(word.id);
    }

    // Connect table's cellChanged() signal to our custom slot onWordEdited
    connect(this, &QTableWidget::cellChanged, this, &WordsTableWidget::onWordEdited);
}

void WordsTableWidget::createUi()
{
    // Set 2 columns with names of language A and language B
    setColumnCount(2);
    setHorizontalHeaderLabels({
        QString(m_database.getLanguageA().c_str()),
        QString(m_database.getLanguageB().c_str())
        });
    // Set stylesheet and other styling options
    setStyleSheet(ResourceManager::getTableWidgetStylesheet().c_str());
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Fill table widget with words from database
    const std::vector<Word>& words = m_database.getWords();
    update(words);
}

void WordsTableWidget::onWordEdited(int row, int col)
{
    QTableWidgetItem* tableItem = item(row, col);
    if (!tableItem)
    {
        WL_LOG_ERRORF("User edited an invalid item of the words table, out of bounds.");
        return;
    }
    // Get the new string value that user has entered
    const std::string editedStr = tableItem->text().toStdString();
    // Get ID of edited word
    if (row < 0 || row >= m_wordsIds.size())
    {
        WL_LOG_ERRORF("Cannot get ID of edited word, row out of range.");
        return;
    }
    const int wordId = m_wordsIds[row];
    // If user has edited word's term A
    if (col == 0)
    {
        // Edit word's term A in database
        if (!m_database.editWordTermA(wordId, editedStr))
        {
            WL_LOG_ERRORF("Failed to edit word's term A in database.");
        }
    }
    // If user has edited word's term B
    else if (col == 1)
    {
        // Edit word's term B in database
        if (!m_database.editWordTermB(wordId, editedStr))
        {
            WL_LOG_ERRORF("Failed to edit word's term B in database.");
        }
    }
    else
    {
        WL_LOG_ERRORF("User edited an invalid item of the words table, column out of bounds.");
        return;
    }
    // Update words table in UI with the words of the currently selected word set.
    // When user edited some word's property in UI, that new property will be shown there without us doing anything,
    // but in case word failed to update in database,
    // we would want to show the old property in UI, to indicate to user that the property they entered did not succeed.
    // In both cases, we just want UI to contain the current properties of words from database, so just do update here.
    const int selectedWordSetId = m_getSelectedWordSetIdCallback();
    const std::vector<Word>& words = m_database.getWordsFromWordSet(selectedWordSetId);
    update(words);
}
