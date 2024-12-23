#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

#include <string>

namespace WordLearner
{

    // Dialog that opens when user wants to edit a word's note.
    // It consists of a text edit where user can edit current note,
    // and an Ok button for saving new note and closing the dialog.
    class EditNoteDialog : public QDialog
    {
        Q_OBJECT

    public:
        EditNoteDialog(const std::string& oldNote, QWidget* parent = nullptr);

    signals:
        // Signal that's emitted with new note when user has finished with this dialog
        void editNote(const std::string& newNote);

    private slots:
        // Slot that's called when user clicks on "Ok" button
        void onOkButtonClicked();

    private: /* functions */

        // Creates all UI elements of the dialog
        void createUi(const std::string& oldNote);

    private: /* variables*/

        // Struct for holding together dialog's UI elements
        struct Ui
        {
            // Dialog's primary layout
            QVBoxLayout* layout = nullptr;
            // Text edit and label for editing note
            QLabel* noteLabel = nullptr;
            QTextEdit* noteTextEdit = nullptr;
            // Button for saving new note and closing dialog
            QPushButton* okButton = nullptr;
        } ui;
    };

} // namespace WordLearner