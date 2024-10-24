#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "modules/image_processor.h"
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , imageProcessor(new ImageProcessor())  // Initialize ImageProcessor
{
    ui->setupUi(this);
    imageWidget = new ImageWidget(this);
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(ui->imporbtn);
    layout->addWidget(imageWidget, 0, Qt::AlignCenter);

    // Create a central widget to hold the layout
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);


}

MainWindow::~MainWindow()
{
    delete ui;
    delete imageProcessor;
    delete imageWidget;
}

void MainWindow::on_imporbtn_clicked()
{
    std::optional<QPixmap> pixmap = imageProcessor->loadAndPrepareImage("/Users/osama/Developer/SiemensFinalProj/SEM-Image-Viewer/Utils/micro-electronic-sed.jpg", imageWidget->size());

    if (pixmap) {
        // Set the processed and scaled pixmap into the custom ImageWidget
        imageWidget->setImage(*pixmap);
    } else {
        // Display error message in case of failure
        QMessageBox::warning(this, "Error", "Failed to load the image.");
    }
}
