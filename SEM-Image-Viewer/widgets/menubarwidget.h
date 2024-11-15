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
#include "WidgetViewController.h"

#include<iostream>

using namespace std;



class MenuBarWidget : public QMenuBar
{
    Q_OBJECT
public:
    explicit MenuBarWidget(WidgetViewController* widgetViewController, QWidget *parent = nullptr);

private:
    void fileMenu();
    void editMenu();
    void viewMenu();
    void optionsMenu();

    void saveSession();

signals:
    void exportProgressUpdated();
    void exportStarted(int maxIterations);
    void exportFinished();

private slots:
    void showLeftSidebarClicked(bool isChecked);
    void showRightSidebarClicked(bool isChecked);
    void showImageClicked(bool isChecked);
    void showLoggerClicked(bool isChecked);
    void exportImages(QString format);
    void exportSelectedImage(QString format);

    void onLeftSidebarViewChanged(bool state);
    void onRightSidebarViewChanged(bool state);
    void onImageViewChanged(bool state);
    void onLoggerViewChanged(bool state);
    // void exportAll();

private:
    QAction *explorerAction;
    QAction *heatMapAction;
    QAction *intensityPlotAction;
    QAction *showLeftSidebarAction;
    QAction *showRightSidebarAction;
    QAction *showLoggerAction;
    QAction *showImageAction;

    ImageDialog *imageDialog;
    WidgetViewController* viewController;
};

#endif // MENUBARWIDGET_H
