#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QButtonGroup>

class ToolbarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolbarWidget(QWidget *parent = nullptr);
    QToolButton *imageViewButton;
    QToolButton *diffViewButton;
    QToolButton *gridViewButton;
    QToolButton *saveButton;
    QToolButton *redoButton;
    QToolButton *undoButton;

private:
    QToolButton *seperatorIcon;

public slots:
    void onSelectDiffView();

private slots:
    void onUndoClicked();
    void onRedoClicked();

signals:
    void undoTriggered();
    void redoTriggered();

};

#endif // TOOLBARWIDGET_H
