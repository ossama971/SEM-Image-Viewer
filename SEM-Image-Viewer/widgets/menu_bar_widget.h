#ifndef MENU_BAR_WIDGET_H
#define MENU_BAR_WIDGET_H

#include <QMenuBar>
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
    void closeAll();
private slots:
    void exportImages(const QString &format);
    void onThemeActionTriggered();

public slots:
    void onLeftSidebarViewChanged(bool state);
    void onRightSidebarViewChanged(bool state);
    void onImageViewChanged(bool state);
    void onLoggerViewChanged(bool state);
    void exportSelectedImage(const QString &format);

private:
    QAction *explorerAction = nullptr;
    QAction *heatMapAction = nullptr;
    QAction *intensityPlotAction = nullptr;
    QAction *showLeftSidebarAction = nullptr;
    QAction *showRightSidebarAction = nullptr;
    QAction *showLoggerAction = nullptr;
    QAction *showImageAction = nullptr;
    QAction *darkModeAction = nullptr;
    ImageDialog *imageDialog = nullptr;
    bool isDarkMode = false;
};

#endif // MENU_BAR_WIDGET_H
