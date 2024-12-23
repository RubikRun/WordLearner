#define WL_FILENAME "WordsTableWidget.cpp"

#include "WordsTableWidget.h"

#include "EditNoteDialog.h"
#include "Database.h"
#include "ResourceManager.h"
#include "Logger.hpp"
#include "StringUtils.h"

#include <QHeaderView>
#include <QMenu>

using namespace WordLearner;

WordsTableWidget::WordsTableWidget(QWidget* parent)
	: QTableWidget(parent)
{}

void WordsTableWidget::init(Database* database, std::function<int()> getSelectedWordSetIdCallback)
{
    m_database = database;
    m_getSelectedWordSetIdCallback = getSelectedWordSetIdCallback;
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

void WordsTableWidget::update()
{
    // Get words of selected word set
    const int selectedWordSetId = m_getSelectedWordSetIdCallback();
    const std::vector<Word>& words = m_database->getWordsFromWordSet(selectedWordSetId);
    // Update table widget to contain those words
    update(words);
}

void WordsTableWidget::createUi()
{
    if (m_database == nullptr)
    {
        WL_LOG_ERRORF("Trying to create UI of words table widget but no database is given. Try properly initializing it.");
        return;
    }
    setContextMenuPolicy(Qt::CustomContextMenu);
    // Connect context menu signal with our custom slot for showing the context menu
    connect(this, &QTableWidget::customContextMenuRequested, this, &WordsTableWidget::showContextMenu);
    // Set 2 columns with names of language A and language B
    setColumnCount(2);
    setHorizontalHeaderLabels({
        QString(m_database->getLanguageA().c_str()),
        QString(m_database->getLanguageB().c_str())
        });
    // Set stylesheet and other styling options
    setStyleSheet(ResourceManager::getTableWidgetStylesheet().c_str());
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    // Fill table widget with words from database
    const std::vector<Word>& words = m_database->getWords();
    update(words);
}

void WordsTableWidget::showContextMenu(const QPoint& pos)
{
    // Get the item at the clicked position
    const QTableWidgetItem* item = itemAt(pos);
    // If there is no item under the cursor
    if (item == nullptr)
    {
        // just don't show context menu
        return;
    }

    // Get word's index
    const int wordIndex = item->row();
    if (wordIndex < 0 || wordIndex >= m_wordsIds.size())
    {
        WL_LOG_ERRORF("Trying to show a context menu for a word in table but word's index is invalid.");
        return;
    }
    // Get word's ID
    const int wordId = m_wordsIds[wordIndex];

    // Create context menu
    QMenu contextMenu(this);
    // Create an action for editing a word's note
    QAction* editNoteAction = contextMenu.addAction("Edit note");
    // Connect action's triggered slot with our custom slot for editing a note
    connect(editNoteAction, &QAction::triggered, this, [this, wordId]() { onEditNoteOptionClicked(wordId); });

    // Show the menu at the global position of the cursor
    contextMenu.exec(viewport()->mapToGlobal(pos));
}

void WordsTableWidget::onEditNoteOptionClicked(int wordId)
{
    if (m_database == nullptr)
    {
        WL_LOG_ERRORF("Trying to edit a word's note but words table widget is not yet initialized.");
        return;
    }

    // Find word from given word ID.
    // NOTE: Database needs to be casted to const pointer,
    //       because otherwise it looks for the non-const version of findWord() which is private.
    const Word* word = static_cast<const Database*>(m_database)->findWord(wordId);
    if (word == nullptr)
    {
        WL_LOG_ERRORF("Trying to edit a word's note but word with given ID doesn't exist in database.");
        return;
    }
    const std::string& oldNote = word->note;

    // Create dialog for editing a note
    EditNoteDialog editNoteDialog(oldNote, this);
    // Connect dialog's editNote() signal to our onNoteEdited() slot here
    connect
    (
        &editNoteDialog,
        &EditNoteDialog::editNote,
        this,
        // NOTE: std::bind() doesn't work here for some reason, so use lambda instead
        [this, wordId](const std::string& newNote)
        {
            this->onNoteEdited(wordId, newNote);
        }
    );
    // Open dialog
    editNoteDialog.exec();
}

void WordsTableWidget::onNoteEdited(int wordId, const std::string& newNote)
{
    if (!m_database->editWordNote(wordId, newNote))
    {
        WL_LOG_ERRORF("Failed to edit a word's note.");
    }
    // Update words in UI to show the new note
    update();
}

void WordsTableWidget::onWordEdited(int row, int col)
{
    if (m_database == nullptr)
    {
        WL_LOG_ERRORF("Trying to edit a word but words table widget is not yet initialized.");
        return;
    }
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
        if (!m_database->editWordTermA(wordId, editedStr))
        {
            WL_LOG_ERRORF("Failed to edit word's term A in database.");
        }
    }
    // If user has edited word's term B
    else if (col == 1)
    {
        // Edit word's term B in database
        if (!m_database->editWordTermB(wordId, editedStr))
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
    update();
}
