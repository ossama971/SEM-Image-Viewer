#ifndef SAVE_SESSION_DIALOG_H
#define SAVE_SESSION_DIALOG_H

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
    void cancel();

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

#endif // SAVE_SESSION_DIALOG_H
