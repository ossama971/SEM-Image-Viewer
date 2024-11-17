#ifndef MENUBARWIDGET_H
#define MENUBARWIDGET_H

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QWidget>
#include <QMainWindow>
#include <QObject>
#include <QImage>
#include <QFileDialog>
#include <iostream>
#include <thread>
#include <vector>
#include <QMetaObject>
#include <QThread>

#include "../core/engines/Workspace.h"
#include "../core/data/Image.h"
#include "../core/data/ImageFormat.h"
#include "ImageDialog.h"

#include<iostream>

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

signals:
    void showLeftSidebarClicked(bool isChecked);
    void showRightSidebarClicked(bool isChecked);
    void showLoggerClicked(bool isChecked);
    void showImageClicked(bool isChecked);
    void exportProgressUpdated();
    void exportStarted(int maxIterations);
    void exportFinished();
    void themeToggled();
private slots:
    void exportImages(QString format);
    void onThemeActionTriggered();
    // void exportAll();

public slots:
    void onLeftSidebarViewChanged(bool state);
    void onRightSidebarViewChanged(bool state);
    void onImageViewChanged(bool state);
    void onLoggerViewChanged(bool state);
    void exportSelectedImage(QString format);

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

#endif // MENUBARWIDGET_H
