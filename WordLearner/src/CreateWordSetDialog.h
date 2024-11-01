#pragma once

#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QLabel>

#include <string>

namespace WordLearner
{

    // Dialog that opens when user wants to create a new word set.
    // It consists of 1 line edit where user can enter name of the new word set.
    class CreateWordSetDialog : public QDialog
    {
        Q_OBJECT

    public:
        CreateWordSetDialog(QWidget* parent = nullptr);

    signals:
        // Signal that's emitted with new word set's data when user has finished with this dialog
        void createWordSet(const std::string& name);

    private slots:
        // Slot that's called when user clicks on "Create word set" button
        void onCreateWordSetButtonClicked();

    private: /* functions */

        // Creates all UI elements of the dialog
        void createUi();

    private: /* variables*/

        // Struct for holding together dialog's UI elements
        struct Ui
        {
            // Dialog's primary layout
            QVBoxLayout* layout = nullptr;
            // Line edit and label for name of new word set
            QLabel* nameLabel = nullptr;
            QLineEdit* nameLineEdit = nullptr;
            // Button for creating word set with entered data
            QPushButton* createWordSetButton = nullptr;
        } ui;
    };

} // namespace WordLearner