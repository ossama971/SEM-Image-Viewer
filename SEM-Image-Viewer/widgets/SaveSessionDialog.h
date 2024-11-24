#ifndef SAVESESSIONDIALOG_H
#define SAVESESSIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <filesystem>

class SaveSessionDialog : public QDialog {
    Q_OBJECT

public:
    explicit SaveSessionDialog(QWidget *parent = nullptr);
    std::filesystem::path getDirectoryPath() const;
    std::filesystem::path getJsonFilePath() const;

private slots:
    void browseForFolder();
    void save();

private:
    QLabel *folderLabel;
    QLabel *sessionFolderLabel;
    QLabel *fileLabel;
    QLineEdit *folderEdit;
    QLineEdit *sessionFolderEdit;
    QLineEdit *fileEdit;
    QPushButton *browseButton;
    QPushButton *saveButton;
    QPushButton *cancelButton;

    std::filesystem::path directoryPath;
    std::filesystem::path sessionFolderPath;
    std::filesystem::path jsonFilePath;
};

#endif // SAVESESSIONDIALOG_H
