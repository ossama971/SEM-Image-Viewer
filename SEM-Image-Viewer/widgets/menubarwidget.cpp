#include "menubarwidget.h"

MenuBarWidget::MenuBarWidget(QMenuBar *parent) : QMenuBar(parent) {

    fileMenu();
    editMenu();
    viewMenu();
    optionsMenu();

}

void MenuBarWidget::fileMenu(){

    QMenu *fileMenu = this->addMenu("File");
    QMenu *exportMenu = new QMenu("Export", fileMenu);

    QAction *openImageAction = new QAction("Open Image", this);
    QAction *openFolderAction = new QAction("Open Forlder", this);
    QAction *PDFAction = new QAction("PDF", this);
    QAction *PNGAction = new QAction("PNG", this);

    exportMenu->addAction(PDFAction);
    exportMenu->addAction(PNGAction);

    fileMenu->addAction(openImageAction);
    fileMenu->addAction(openFolderAction);
    fileMenu->addSeparator();
    fileMenu->addMenu(exportMenu);

}

void MenuBarWidget::editMenu(){
    QMenu *editMenu = this->addMenu("Edit");

    QAction *undoAction = new QAction("Undo", this);
    QAction *redoAction = new QAction("Redo", this);
    QAction *saveAction = new QAction("Save", this);
    QAction *findAction = new QAction("Find In Files", this);

    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addAction(saveAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
}

void MenuBarWidget::viewMenu(){
    QMenu *viewMenu = this->addMenu("View");

    QAction *explorerAction = new QAction("Explorer", this);
    QAction *heatMapAction = new QAction("HeatMap", this);
    QAction *intensityPlotAction = new QAction("Intensity Plot", this);
    QAction *showLeftSidebarAction = new QAction("Show Left Sidebar", this);
    QAction *showRightSidebarAction = new QAction("Show Right Sidebar", this);
    QAction *showLoggerAction = new QAction("Show Logger Sidebar", this);
    QAction *showImageAction = new QAction("Show Image Sidebar", this);

    showLeftSidebarAction->setCheckable(true);
    showRightSidebarAction->setCheckable(true);
    showLoggerAction->setCheckable(true);
    showImageAction->setCheckable(true);


    // showLeftSidebarAction->setChecked(false);
    connect(showLeftSidebarAction, &QAction::triggered, this, &MenuBarWidget::showLeftSidebarClicked);
    connect(showRightSidebarAction, &QAction::triggered, this, &MenuBarWidget::showRightSidebarClicked);
    connect(showLoggerAction, &QAction::triggered, this, &MenuBarWidget::showLoggerClicked);
    connect(showImageAction, &QAction::triggered, this, &MenuBarWidget::showImageClicked);

    viewMenu->addAction(showLeftSidebarAction);
    viewMenu->addAction(showRightSidebarAction);
    viewMenu->addAction(showLoggerAction);
    viewMenu->addAction(showImageAction);
    viewMenu->addSeparator();
    viewMenu->addAction(explorerAction);
    viewMenu->addSeparator();
    viewMenu->addAction(heatMapAction);
    viewMenu->addAction(intensityPlotAction);
}

void MenuBarWidget::optionsMenu(){
    QMenu *optionsMenu = this->addMenu("Options");
    QMenu *fontMenu = new QMenu("Font", optionsMenu);

    QAction *darkModeAction = new QAction("Dark Mode", this);

    optionsMenu->addAction(darkModeAction);
    optionsMenu->addMenu(fontMenu);
}
