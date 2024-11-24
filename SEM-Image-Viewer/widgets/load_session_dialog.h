#ifndef LOAD_SESSION_DIALOG_H
#define LOAD_SESSION_DIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <filesystem>
#include <QPushButton>

class LoadSessionDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoadSessionDialog(QWidget *parent = nullptr);

    std::filesystem::path getJsonFilePath() const;

private slots:
    void browseForFile();

    void load();

private:
    QLabel *fileLabel;
    QLineEdit *fileEdit;
    QPushButton *browseButton;
    QPushButton *loadButton;
    QPushButton *cancelButton;

    std::filesystem::path jsonFilePath;
};

#endif // LOAD_SESSION_DIALOG_H

