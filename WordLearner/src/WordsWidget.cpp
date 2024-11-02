#define WL_FILENAME "WordsWidget.cpp"

#include "WordsWidget.h"

#include "WordsTableWidget.h"
#include "CreateWordDialog.h"
#include "Database.h"
#include "Logger.hpp"

using namespace WordLearner;

WordsWidget::WordsWidget(QWidget* parent)
	: QWidget(parent)
{}

void WordsWidget::init(Database* database, std::function<int()> getSelectedWordSetIdCallback)
{
    m_database = database;
    m_getSelectedWordSetIdCallback = getSelectedWordSetIdCallback;
    createUi();
}

void WordsWidget::update(const std::vector<Word>& words)
{
    ui.wordsTableWidget->update(words);
}

void WordsWidget::createUi()
{
    if (m_database == nullptr)
    {
        WL_LOG_ERRORF("Trying to create UI of words widget but no database is given. Try properly initializing it.");
        return;
    }
	// Create layout
	ui.layout = new QVBoxLayout;
	setLayout(ui.layout);
		
	// Create words table widget
	ui.wordsTableWidget = new WordsTableWidget(this);
    ui.wordsTableWidget->init(m_database, m_getSelectedWordSetIdCallback);
	ui.layout->addWidget(ui.wordsTableWidget);
	// Create button for adding words
	ui.createWordButton = new QPushButton("New");
	ui.layout->addWidget(ui.createWordButton);
	connect(ui.createWordButton, &QPushButton::released, this, &WordsWidget::onCreateWordButtonPressed);
}

void WordsWidget::onCreateWord(const std::string& termA, const std::string& termB, const std::string& note)
{
    if (m_database == nullptr)
    {
        WL_LOG_ERRORF("Trying to create a word but words widget is not yet initialized.");
        return;
    }
    // Create word in database
    const int wordId = m_database->createWord(termA, termB, note);
    if (wordId < 0)
    {
        WL_LOG_ERRORF("Failed to create new word in database.");
        return;
    }
    // Get selected word set's ID
    const int wordSetId = m_getSelectedWordSetIdCallback();
    if (wordSetId < 0)
    {
        WL_LOG_ERRORF("Trying to add new word to selected word set, but there is no selected word set.");
        return;
    }
    // Add new word to selected word set.
    // If selected word set is the global word set,
    // we don't need to explicitly add new word to it,
    // because it's automatically added when created.
    if (wordSetId != 0 && !m_database->addWordToWordSet(wordId, wordSetId))
    {
        WL_LOG_ERRORF("Cannot add new word to selected word set.");
    }
    // Update words table in UI so that it shows the new word
    const std::vector<Word> words = m_database->getWordsFromWordSet(wordSetId);
    ui.wordsTableWidget->update(words);
}

void WordsWidget::onCreateWordButtonPressed()
{
    if (m_database == nullptr)
    {
        WL_LOG_ERRORF("Trying to create a word but words widget is not yet initialized.");
        return;
    }
	// Create dialog for creating a new word
	CreateWordDialog createWordDialog(m_database->getLanguageA(), m_database->getLanguageB(), this);
	// Connect dialog's createWord() signal to our onCreateWord() slot here
	connect(&createWordDialog, &CreateWordDialog::createWord, this, &WordsWidget::onCreateWord);
	// Open dialog
	createWordDialog.exec();
}
