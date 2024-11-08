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
    // QMenu *exportAllMenu = new QMenu("Export All", fileMenu);

    QAction *openImageAction = new QAction("Open Image", this);
    QAction *openFolderAction = new QAction("Open Forlder", this);
    QAction *JPGAction = new QAction("JPG", this);
    QAction *PNGAction = new QAction("PNG", this);
    QAction *BMPAction = new QAction("BMP", this);

    exportMenu->addAction(JPGAction);
    exportMenu->addAction(PNGAction);
    exportMenu->addAction(BMPAction);

    fileMenu->addAction(openImageAction);
    fileMenu->addAction(openFolderAction);
    fileMenu->addSeparator();
    fileMenu->addMenu(exportMenu);

    connect(JPGAction, &QAction::triggered, this, [=]() { exportImage("*.jpg"); });
    connect(PNGAction, &QAction::triggered, this, [=]() { exportImage("*.png"); });
    connect(BMPAction, &QAction::triggered, this, [=]() { exportImage("*.bmp"); });
}



void MenuBarWidget::exportImage(QString format){
    qDebug("-------------------------------------------------exportImage called-------------------------------------------------");
    QImage image(QSize(128, 128), QImage::Format_ARGB32);
    image.fill(Qt::red); // A red rectangle.

    // Set up the save file dialog with the specified format
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image File"), "Untitled", tr("Images (%1)").arg(format));
    if (!fileName.isEmpty()) {
        image.save(fileName);
    }

    //////////////////////

    // QImage image(QSize(128, 128), QImage::Format_ARGB32);
    // image.fill(Qt::red); // A red rectangle.

    // // Define available formats and set PNG as default
    // QStringList formats = {"PNG", "JPG", "BMP"};
    // QString selectedFormat = "PNG"; // Default format

    // // Create file type filter for each format
    // QStringList filterList;
    // for (const QString &format : formats) {
    //     filterList << QString("%1 Files (*.%2)").arg(format).arg(format.toLower());
    // }
    // QString filter = filterList.join(";;");

    // // Open the save file dialog with format options
    // QString fileName = QFileDialog::getSaveFileName(
    //     this,
    //     tr("Save Image File"),
    //     QString(),
    //     filter,
    //     &selectedFormat // This will store the user's selected format
    //     );

    // if (!fileName.isEmpty()) {
    //     // Append the format to the filename if it's not already there
    //     QString extension = "." + selectedFormat.toLower();
    //     if (!fileName.endsWith(extension, Qt::CaseInsensitive)) {
    //         fileName += extension;
    //     }

    //     // Save the image in the selected format
    //     image.save(fileName, selectedFormat.toUtf8().constData());
    // }

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
