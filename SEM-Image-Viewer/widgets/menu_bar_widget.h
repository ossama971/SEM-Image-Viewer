#ifndef MENU_BAR_WIDGET_H
#define MENU_BAR_WIDGET_H

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QMainWindow>
#include <QObject>
#include <QImage>
#include <QFileDialog>
#include <QMetaObject>

#include "image_dialog.h"

using namespace std;

class MenuBarWidget : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBarWidget(QWidget *parent = nullptr);

private:
    void fileMenu();
    void editMenu();
    void viewMenu();
    void optionsMenu();
    void saveSession();
    void loadSession();

signals:
    void showLeftSidebarClicked(bool isChecked);
    void showRightSidebarClicked(bool isChecked);
    void showLoggerClicked(bool isChecked);
    void showImageClicked(bool isChecked);
    void exportProgressUpdated();
    void exportStarted(int maxIterations);
    void exportFinished();
    void themeToggled();
    void undoChecked();
    void redoChecked();
private slots:
    void exportImages(QString format);
    void onThemeActionTriggered();

public slots:
    void onLeftSidebarViewChanged(bool state);
    void onRightSidebarViewChanged(bool state);
    void onImageViewChanged(bool state);
    void onLoggerViewChanged(bool state);
    void exportSelectedImage(const QString format);

private:
    QAction *explorerAction;
    QAction *heatMapAction;
    QAction *intensityPlotAction;
    QAction *showLeftSidebarAction;
    QAction *showRightSidebarAction;
    QAction *showLoggerAction;
    QAction *showImageAction;
    QAction *darkModeAction;
    ImageDialog *imageDialog;
    bool isDarkMode = false;
};

#endif // MENU_BAR_WIDGET_H
