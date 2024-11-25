#include "exit_dialog_widget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

ExitDialogWidget::ExitDialogWidget(QWidget *parent) : QDialog(parent) {

    setWindowFlags(windowFlags() & ~Qt::WindowTitleHint);
    resize(350, 200);

    // Create a label with a larger font for the main message
    auto *mainLabel = new QLabel("You have unsaved changes. Do you want to save them before exiting?");
    mainLabel->setAlignment(Qt::AlignCenter);

    QFont mainFont;
    mainFont.setPointSize(12);
    mainLabel->setFont(mainFont);

    // Create a label for the warning message
    auto *warningLabel = new QLabel("Your changes will be lost if you don't save them.");
    warningLabel->setAlignment(Qt::AlignCenter);
    warningLabel->setStyleSheet("color: #a0a0a0;");

    // Set up icons for the buttons
    auto *saveButton = new QPushButton("Save");
    saveButton->setStyleSheet(
        "QPushButton {"
        "    padding: 6px 12px;"
        "    background-color: #4CAF50;"  // Dark green for Save button
        "    color: white;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #45a049;"
        "}"
        );
    auto *dontSaveButton = new QPushButton("Don't Save");
    auto *cancelButton = new QPushButton("Cancel");
    dontSaveButton->setStyleSheet(
        "QPushButton {"
        "    padding: 6px 12px;"
        "    background-color: #FF5733;"  // Dark red for Don't Save button
        "    color: white;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e74c3c;"
        "}"
        );
    cancelButton->setStyleSheet(
        "QPushButton {"
        "    padding: 6px 12px;"
        "    background-color: #555555;"  // Dark gray for Cancel button
        "    color: white;"
        "    border-radius: 4px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #666666;"
        "}"
        );

    // Add padding and margin for the buttons
    saveButton->setMinimumHeight(30);
    dontSaveButton->setMinimumHeight(30);
    cancelButton->setMinimumHeight(30);

    // Connect the buttons to their respective slots
    connect(saveButton, &QPushButton::clicked, this, &ExitDialogWidget::onSaveClicked);
    connect(dontSaveButton, &QPushButton::clicked, this, &ExitDialogWidget::onDontSaveClicked);
    connect(cancelButton, &QPushButton::clicked, this, &ExitDialogWidget::onCancelClicked);

    // Set up the layout
    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(dontSaveButton);
    buttonLayout->addWidget(cancelButton);

    auto *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(mainLabel);
    mainLayout->addWidget(warningLabel);
    mainLayout->addSpacing(10);
    mainLayout->addLayout(buttonLayout);
    mainLayout->setContentsMargins(20, 20, 20, 20);

    setLayout(mainLayout);

    // Set dialog background color and styling
    setStyleSheet("QDialog { border-radius: 8px; }");

}

void ExitDialogWidget::onSaveClicked() {
    emit saveRequested();
    reject();
}

void ExitDialogWidget::onDontSaveClicked() {
    emit dontSaveRequested();
    accept();  // Close the dialog to proceed with application close without saving
}

void ExitDialogWidget::onCancelClicked() {
    reject();  // Close the dialog without closing the application
}

