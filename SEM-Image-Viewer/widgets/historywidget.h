#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStack>

class HistoryWidget : public QWidget {
    Q_OBJECT

public:
    explicit HistoryWidget(QWidget *parent = nullptr);

private slots:
    void undoAction();
    void redoAction();
    void addAction(const QString &action); // Function to add actions to history

private:
    QLabel *headerLabel;
    QPushButton *undoButton;
    QPushButton *redoButton;
    QListWidget *actionList;
    QStack<QString> undoStack;
    QStack<QString> redoStack;

    void updateActionList();
};

#endif
