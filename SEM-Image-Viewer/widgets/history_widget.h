#ifndef HISTORY_WIDGET_H
#define HISTORY_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QToolButton>

class HistoryWidget : public QWidget {
    Q_OBJECT

public:
    explicit HistoryWidget(QWidget *parent = nullptr);

signals:
    void undo();
    void redo();
private:
    QLabel *headerLabel = nullptr;
    QPushButton *undoButton = nullptr;
    QPushButton *redoButton = nullptr;
    QListWidget *actionList = nullptr;
    QToolButton* toggleButton = nullptr;
    QWidget* listContainer = nullptr;
public slots:
    void loadActionList(const QList<QString> &actions);
    void updateActionList(const QString &action);
    void popAction();
public slots:
    void undoAction();
    void redoAction();
private slots:
    void showAndHideList();

};

#endif
