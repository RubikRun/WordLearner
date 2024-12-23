#include "EditNoteDialog.h"

using namespace WordLearner;

EditNoteDialog::EditNoteDialog(const std::string& oldNote, QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Edit word's note");
    setMinimumSize(300, 200);
    createUi(oldNote);
}

void EditNoteDialog::onOkButtonClicked() {
    // Retrieve entered note from text edit
    const std::string newNote = ui.noteTextEdit->toPlainText().toStdString();
    // Emit the signal with new note
    emit editNote(newNote);
    // Close the dialog
    accept();
}

void WordLearner::EditNoteDialog::createUi(const std::string& note)
{
    // Create layout
    ui.layout = new QVBoxLayout;
    setLayout(ui.layout);
    // Create a label and a text edit for note
    ui.noteLabel = new QLabel("Note", this);
    ui.noteTextEdit = new QTextEdit(this);
    ui.noteTextEdit->setText(note.c_str());
    ui.layout->addWidget(ui.noteLabel);
    ui.layout->addWidget(ui.noteTextEdit);
    // Create a button for finishing and updating note
    ui.okButton = new QPushButton("Ok", this);
    ui.layout->addWidget(ui.okButton);
    // Connect button to corresponding slot
    connect(ui.okButton, &QPushButton::clicked, this, &EditNoteDialog::onOkButtonClicked);
}
