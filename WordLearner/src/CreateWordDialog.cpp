#include "CreateWordDialog.h"

using namespace WordLearner;

CreateWordDialog::CreateWordDialog(const std::string& languageA, const std::string& languageB, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Create new word");
    setMinimumSize(300, 200);
    createUi(languageA, languageB);
}

void CreateWordDialog::onCreateWordButtonClicked() {
    // Retrieve entered term A, term B and note from line edits
    const std::string termA = ui.termALineEdit->text().toStdString();
    const std::string termB = ui.termBLineEdit->text().toStdString();
    const std::string note = ui.noteLineEdit->text().toStdString();
    // Emit the signal with the entered data
    emit createWord(termA, termB, note);
    // Close the dialog
    accept();
}

void WordLearner::CreateWordDialog::createUi(const std::string& languageA, const std::string& languageB)
{
    // Create layout
    ui.layout = new QVBoxLayout;
    setLayout(ui.layout);
    // Create a label and a line edit for term A
    ui.termALabel = new QLabel((std::string("Word in ") + languageA).c_str(), this);
    ui.termALineEdit = new QLineEdit(this);
    ui.layout->addWidget(ui.termALabel);
    ui.layout->addWidget(ui.termALineEdit);
    // Create a label and a line edit for term B
    ui.termBLabel = new QLabel((std::string("Word in ") + languageB).c_str(), this);
    ui.termBLineEdit = new QLineEdit(this);
    ui.layout->addWidget(ui.termBLabel);
    ui.layout->addWidget(ui.termBLineEdit);
    // Create a label and a line edit for note
    ui.noteLabel = new QLabel("Note", this);
    ui.noteLineEdit = new QLineEdit(this);
    ui.layout->addWidget(ui.noteLabel);
    ui.layout->addWidget(ui.noteLineEdit);
    // Create a button for finishing and creating the word
    ui.createWordButton = new QPushButton("Create word", this);
    ui.layout->addWidget(ui.createWordButton);
    // Connect button to corresponding slot
    connect(ui.createWordButton, &QPushButton::clicked, this, &CreateWordDialog::onCreateWordButtonClicked);
}
