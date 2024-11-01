#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

#include <string>

namespace WordLearner
{

    // Dialog that opens when user wants to create a new word.
    // It consists of 3 line edits where user can enter term A, term B and note of the new word.
    class CreateWordDialog : public QDialog
    {
        Q_OBJECT

    public:
        CreateWordDialog(const std::string languageA, const std::string languageB, QWidget* parent = nullptr);

    signals:
        // Signal that's emitted with new word's data when user has finished with this dialog
        void createWord(const std::string& termA, const std::string& termB, const std::string& note);

    private slots:
        // Slot that's called when user clicks on "Create word" button
        void onCreateWordButtonClicked();

    private: /* functions */
        
        // Creates all UI elements of the dialog
        void createUi(const std::string languageA, const std::string languageB);

    private: /* variables*/

        // Struct for holding together dialog's UI elements
        struct Ui
        {
            // Dialog's primary layout
            QVBoxLayout* layout = nullptr;
            // Line edit and label for term A of new word
            QLabel* termALabel = nullptr;
            QLineEdit* termALineEdit = nullptr;
            // Line edit and label for term B of new word
            QLabel* termBLabel = nullptr;
            QLineEdit* termBLineEdit = nullptr;
            // Line edit and label for note of new word
            QLabel* noteLabel = nullptr;
            QLineEdit* noteLineEdit = nullptr;
            // Button for creating word with entered data
            QPushButton* createWordButton = nullptr;
        } ui;
    };

} // namespace WordLearner