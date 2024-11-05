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
    void addAction(const QString &action);
signals:
    void undo();
    void redo();
private:
    QLabel *headerLabel;
    QPushButton *undoButton;
    QPushButton *redoButton;
    QListWidget *actionList;
    QStack<QString> undoStack;
    QStack<QString> redoStack;

    void updateActionList();

private slots:
    void undoAction();
    void redoAction();

};

#endif
