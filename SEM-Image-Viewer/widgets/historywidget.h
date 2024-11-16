#ifndef HISTORYWIDGET_H
#define HISTORYWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QStack>
#include <QToolButton>
class HistoryWidget : public QWidget {
    Q_OBJECT

public:
    explicit HistoryWidget(QWidget *parent = nullptr);

signals:
    void undo();
    void redo();
private:
    QLabel *headerLabel;
    QPushButton *undoButton;
    QPushButton *redoButton;
    QListWidget *actionList;
    QToolButton* toggleButton;
    QWidget* listContainer;
public slots:
    void loadActionList(QList<QString> actions);
    void updateActionList(QString action);
    void popAction();
public slots:
    void undoAction();
    void redoAction();
private slots:
    void showAndHideList();

};

#endif
