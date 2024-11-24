#include "SaveSessionDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QString>
#include <filesystem>

SaveSessionDialog::SaveSessionDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Save Session");
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);

    // Create a grid layout for the labels and inputs
    auto *gridLayout = new QGridLayout();

    // Base Directory Input
    folderLabel = new QLabel("Base Directory:", this);
    folderEdit = new QLineEdit(this);
    browseButton = new QPushButton("Browse...", this);

    gridLayout->addWidget(folderLabel, 0, 0); // Row 0, Column 0
    gridLayout->addWidget(folderEdit, 0, 1); // Row 0, Column 1
    gridLayout->addWidget(browseButton, 0, 2); // Row 0, Column 2

    // Session Folder Name Input
    sessionFolderLabel = new QLabel("Session Folder Name:", this);
    sessionFolderEdit = new QLineEdit("session_data", this);

    gridLayout->addWidget(sessionFolderLabel, 1, 0); // Row 1, Column 0
    gridLayout->addWidget(sessionFolderEdit, 1, 1); // Row 1, Column 1

    // JSON File Name Input
    fileLabel = new QLabel("JSON File Name:", this);
    fileEdit = new QLineEdit("session.json", this);

    gridLayout->addWidget(fileLabel, 2, 0); // Row 2, Column 0
    gridLayout->addWidget(fileEdit, 2, 1); // Row 2, Column 1

    // Buttons Layout
    auto *buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save", this);
    cancelButton = new QPushButton("Cancel", this);

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    connect(browseButton, &QPushButton::clicked, this, &SaveSessionDialog::browseForFolder);
    connect(saveButton, &QPushButton::clicked, this, &SaveSessionDialog::save);
    connect(cancelButton, &QPushButton::clicked, this, &SaveSessionDialog::cancel);

    // Add layouts to the main layout
    mainLayout->addLayout(gridLayout);
    mainLayout->addLayout(buttonLayout);
}

std::filesystem::path SaveSessionDialog::getDirectoryPath() const {
    return sessionFolderPath;
}

std::filesystem::path SaveSessionDialog::getJsonFilePath() const {
    return jsonFilePath;
}

void SaveSessionDialog::browseForFolder() {
    QString folder = QFileDialog::getExistingDirectory(this, "Select Base Directory");
    if (!folder.isEmpty()) {
        folderEdit->setText(folder);
    }
}

void SaveSessionDialog::save() {
    QString folder = folderEdit->text();
    QString sessionFolderName = sessionFolderEdit->text();
    QString fileName = fileEdit->text();

    if (folder.isEmpty() || sessionFolderName.isEmpty() || fileName.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Base directory, session folder name, and JSON file name must all be specified.");
        return;
    }

    if (!fileName.endsWith(".json")) {
        QMessageBox::warning(this, "Invalid File Name", "The file name must have a .json extension.");
        return;
    }

    sessionFolderPath = std::filesystem::path(folder.toStdString()) / std::filesystem::path(sessionFolderName.toStdString());
    jsonFilePath = sessionFolderPath / std::filesystem::path(fileName.toStdString());

    // Check if the session folder already exists
    if (std::filesystem::exists(sessionFolderPath)) {
        QMessageBox::warning(this, "Folder Exists", QString("The folder '%1' already exists in the selected directory. Please choose a different name.").arg(sessionFolderName));
        return;
    }

    accept();
}

void SaveSessionDialog::cancel(){
    reject();
}
