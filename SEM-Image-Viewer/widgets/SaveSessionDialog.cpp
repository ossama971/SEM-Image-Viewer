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

    auto *folderLayout = new QHBoxLayout();
    folderLabel = new QLabel("Save Directory:", this);
    folderEdit = new QLineEdit(this);
    browseButton = new QPushButton("Browse...", this);

    folderLayout->addWidget(folderLabel);
    folderLayout->addWidget(folderEdit);
    folderLayout->addWidget(browseButton);

    connect(browseButton, &QPushButton::clicked, this, &SaveSessionDialog::browseForFolder);

    auto *fileLayout = new QHBoxLayout();
    fileLabel = new QLabel("JSON File Name:", this);
    fileEdit = new QLineEdit("session.json", this);

    fileLayout->addWidget(fileLabel);
    fileLayout->addWidget(fileEdit);

    auto *buttonLayout = new QHBoxLayout();
    saveButton = new QPushButton("Save", this);
    cancelButton = new QPushButton("Cancel", this);

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);

    connect(saveButton, &QPushButton::clicked, this, &SaveSessionDialog::save);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    mainLayout->addLayout(folderLayout);
    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(buttonLayout);
}

std::filesystem::path SaveSessionDialog::getDirectoryPath() const {
    return directoryPath;
}

std::filesystem::path SaveSessionDialog::getJsonFilePath() const {
    return jsonFilePath;
}

void SaveSessionDialog::browseForFolder() {
    QString folder = QFileDialog::getExistingDirectory(this, "Select Save Directory");
    if (!folder.isEmpty()) {
        folderEdit->setText(folder);
    }
}

void SaveSessionDialog::save() {
    QString folder = folderEdit->text();
    QString fileName = fileEdit->text();

    if (folder.isEmpty() || fileName.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Both save directory and JSON file name must be specified.");
        return;
    }

    if (!fileName.endsWith(".json")) {
        QMessageBox::warning(this, "Invalid File Name", "The file name must have a .json extension.");
        return;
    }

    directoryPath = std::filesystem::path(folder.toStdString());
    jsonFilePath = directoryPath / std::filesystem::path(fileName.toStdString());

    accept();
}
