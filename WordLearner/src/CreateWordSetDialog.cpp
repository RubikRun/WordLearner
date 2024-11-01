#include "CreateWordSetDialog.h"

using namespace WordLearner;

CreateWordSetDialog::CreateWordSetDialog(QWidget* parent)
    : QDialog(parent)
{
    setWindowTitle("Create new word set");
    setMinimumSize(300, 100);
    createUi();
}

void CreateWordSetDialog::onCreateWordSetButtonClicked() {
    // Retrieve entered name from line edit
    const std::string name = ui.nameLineEdit->text().toStdString();
    // Emit the signal with the entered data
    emit createWordSet(name);
    // Close the dialog
    accept();
}

void WordLearner::CreateWordSetDialog::createUi()
{
    // Create layout
    ui.layout = new QVBoxLayout;
    setLayout(ui.layout);
    // Create a label and a line edit for name
    ui.nameLabel = new QLabel("Name", this);
    ui.nameLineEdit = new QLineEdit(this);
    ui.layout->addWidget(ui.nameLabel);
    ui.layout->addWidget(ui.nameLineEdit);
    // Create a button for finishing and creating the word set
    ui.createWordSetButton = new QPushButton("Create word set", this);
    ui.layout->addWidget(ui.createWordSetButton);
    // Connect button to corresponding slot
    connect(ui.createWordSetButton, &QPushButton::clicked, this, &CreateWordSetDialog::onCreateWordSetButtonClicked);
}
