#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>
#include <QToolButton>

class ToolbarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ToolbarWidget(QWidget *parent = nullptr);
    QToolButton *imageViewButton = nullptr;
    QToolButton *diffViewButton = nullptr;
    QToolButton *gridViewButton = nullptr;
    QToolButton *saveButton = nullptr;
    QToolButton *redoButton = nullptr;
    QToolButton *undoButton = nullptr;
    QToolButton *minimizeToolbarButton = nullptr;
    QToolButton *minimizeLoggerButton = nullptr;

private:
    QToolButton *seperatorIcon = nullptr;
    QToolButton *seperatorIcon2 = nullptr;

public slots:
    void onSelectDiffView();

private slots:
    void onUndoClicked();
    void onRedoClicked();
    void onSaveClicked();
    void onMinimizeLoggerClicked(bool checked);
    void onMinimizeToolbarClicked(bool checked);

signals:
    void undoTriggered();
    void redoTriggered();
    void saveButtonClicked();
    void minimizeLoggerClicked(bool);
    void minimizeToolbarClicked(bool);
};

#endif // TOOLBARWIDGET_H
