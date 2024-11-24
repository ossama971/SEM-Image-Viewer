#include "load_session_dialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>
#include <QString>
#include <filesystem>

LoadSessionDialog::LoadSessionDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Load Session");
    setModal(true);

    auto *mainLayout = new QVBoxLayout(this);

    auto *fileLayout = new QHBoxLayout();
    fileLabel = new QLabel("Select JSON File:", this);
    fileEdit = new QLineEdit(this);
    browseButton = new QPushButton("Browse...", this);

    fileLayout->addWidget(fileLabel);
    fileLayout->addWidget(fileEdit);
    fileLayout->addWidget(browseButton);

    connect(browseButton, &QPushButton::clicked, this, &LoadSessionDialog::browseForFile);

    auto *buttonLayout = new QHBoxLayout();
    loadButton = new QPushButton("Load", this);
    cancelButton = new QPushButton("Cancel", this);

    buttonLayout->addWidget(loadButton);
    buttonLayout->addWidget(cancelButton);

    connect(loadButton, &QPushButton::clicked, this, &LoadSessionDialog::load);
    connect(cancelButton, &QPushButton::clicked, this, &QDialog::reject);

    mainLayout->addLayout(fileLayout);
    mainLayout->addLayout(buttonLayout);
}

std::filesystem::path LoadSessionDialog::getJsonFilePath() const {
    return jsonFilePath;
}

void LoadSessionDialog::browseForFile() {
    QString file = QFileDialog::getOpenFileName(this, "Select Session File", QString(), "JSON Files (*.json)");
    if (!file.isEmpty()) {
        fileEdit->setText(file);
    }
}

void LoadSessionDialog::load() {
    QString file = fileEdit->text();

    if (file.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please select a JSON file.");
        return;
    }

    jsonFilePath = std::filesystem::path(file.toStdString());

    if (jsonFilePath.extension() != ".json") {
        QMessageBox::warning(this, "Invalid File Type", "The selected file must have a .json extension.");
        return;
    }

    accept();
}

