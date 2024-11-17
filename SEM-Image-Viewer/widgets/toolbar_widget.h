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
    QToolButton *minimizeToolbarButton;
    QToolButton *minimizeLoggerButton;

private:
    QToolButton *seperatorIcon;
    QToolButton *seperatorIcon2;

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
