#include "menubarwidget.h"

MenuBarWidget::MenuBarWidget(QWidget *parent) : QMenuBar(parent) {

    fileMenu();
    editMenu();
    viewMenu();
    optionsMenu();

}

void MenuBarWidget::fileMenu(){

    QMenu *fileMenu = this->addMenu("File");
    QMenu *exportAllMenu = new QMenu("Export All", fileMenu);
    QMenu *exportMenu = new QMenu("Export", fileMenu);//selected image

    QAction *openImageAction = new QAction("Open Image", this);
    QAction *openFolderAction = new QAction("Open Forlder", this);

    //selected image
    QAction *JPGAction = new QAction("JPG", this);
    QAction *PNGAction = new QAction("PNG", this);
    QAction *BMPAction = new QAction("BMP", this);

    QAction *JPGAllAction = new QAction("JPG", this);
    QAction *PNGAllAction = new QAction("PNG", this);
    QAction *BMPAllAction = new QAction("BMP", this);

    exportAllMenu->addAction(JPGAllAction);
    exportAllMenu->addAction(PNGAllAction);
    exportAllMenu->addAction(BMPAllAction);

    //selected image
    exportMenu->addAction(JPGAction);
    exportMenu->addAction(PNGAction);
    exportMenu->addAction(BMPAction);

    fileMenu->addAction(openImageAction);
    fileMenu->addAction(openFolderAction);
    fileMenu->addSeparator();
    fileMenu->addMenu(exportMenu);
    fileMenu->addMenu(exportAllMenu);

    connect(JPGAllAction, &QAction::triggered, this, [=]() { exportImages("*.jpg"); });
    connect(PNGAllAction, &QAction::triggered, this, [=]() { exportImages("*.png"); });
    connect(BMPAllAction, &QAction::triggered, this, [=]() { exportImages("*.bmp"); });

    //selected image
    connect(JPGAction, &QAction::triggered, this, [=]() { exportSelectedImage("*.jpg"); });
    connect(PNGAction, &QAction::triggered, this, [=]() { exportSelectedImage("*.png"); });
    connect(BMPAction, &QAction::triggered, this, [=]() { exportSelectedImage("*.bmp"); });
}

void MenuBarWidget::exportSelectedImage(QString format){
    qDebug("-------------------------------------------------exportSelctedImage called-------------------------------------------------");
    Image* image = Workspace::Instance().getActiveSession().getImageRepo().getImage();

    string fileName = image->getPath().filename().string();
    size_t lastDot = fileName.find_last_of('.');

    if (lastDot != string::npos) {
        fileName = fileName.substr(0, lastDot); // Remove the extension
    }

    QString baseName = QString::fromStdString(fileName);

    QString baseFileName = QFileDialog::getSaveFileName(this, tr("Save Image File"), baseName, tr("Images (%1)").arg(format));

    if (!baseFileName.isEmpty()) {
        QFileInfo fileInfo(baseFileName);
        QString extension = fileInfo.completeSuffix();
        QString filePath = fileInfo.path();

        cv::Mat matImg = image->getImageMat();

        QImage qImg = QImage(matImg.data, matImg.cols, matImg.rows, matImg.step[0], QImage::Format_RGB888).rgbSwapped();

        QString numberedFileName = QString("%1/%2.%3").arg(filePath).arg(baseName).arg(extension);

        qImg.save(numberedFileName);
    }
}


void MenuBarWidget::exportImages(QString format) {
    qDebug("-------------------------------------------------exportAllImage called-------------------------------------------------");

    // Collect necessary data
    vector<Image> images = Workspace::Instance().getActiveSession().getImageRepo().getImages();

    QString directoryPath = QFileDialog::getExistingDirectory(this, tr("Select Directory to Save Images"));

    if (directoryPath.isEmpty()) {
        return; // Return early if no folder is chosen
    }

    QFileInfo fileInfo(directoryPath);
    QString extension = format;

    // Create a new thread for exporting images
    QThread* exportThread = new QThread;

    // Move the export logic to the new thread
    connect(exportThread, &QThread::started, [this, directoryPath, extension, images]() {
        unsigned int numCores = std::thread::hardware_concurrency();
        unsigned int imagesPerThread = images.size() / numCores;

        emit exportStarted(images.size());

        auto saveImagesSubset = [&](size_t start, size_t end) {
            for (size_t i = start; i < end; ++i) {

                std::string fileName = images[i].getPath().filename().string();
                size_t lastDot = fileName.find_last_of('.');

                if (lastDot != std::string::npos) {
                    fileName = fileName.substr(0, lastDot); // Remove the extension
                }

                QString baseName = QString::fromStdString(fileName);
                cv::Mat matImg = images[i].getImageMat();
                QImage qImg = QImage(matImg.data, matImg.cols, matImg.rows, matImg.step[0], QImage::Format_RGB888).rgbSwapped();
                QString numberedFileName = QString("%1/%2.%3").arg(directoryPath).arg(baseName).arg(extension);

                qImg.save(numberedFileName);

                // Emit progress update signal from the main thread
                QMetaObject::invokeMethod(this, "exportProgressUpdated", Qt::QueuedConnection);
            }
        };

        std::vector<std::thread> threads;
        for (unsigned int i = 0; i < numCores; ++i) {
            size_t startIdx = i * imagesPerThread;
            size_t endIdx = (i == numCores - 1) ? images.size() : (startIdx + imagesPerThread);
            threads.emplace_back(saveImagesSubset, startIdx, endIdx);
            std::this_thread::sleep_for(std::chrono::milliseconds(5000));
        }

        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }

        // Emit exportFinished() from the main thread
        QMetaObject::invokeMethod(this, "exportFinished", Qt::QueuedConnection);
    });

    // Connect the thread's finished signal to clean up
    connect(exportThread, &QThread::finished, exportThread, &QThread::deleteLater);

    // Start the thread
    exportThread->start();
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
