#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/image_widget.h"
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create the ImageWidget instance
    imageWidget = new ImageWidget(this);

    // Create a layout to hold the ImageWidget
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(imageWidget, 0, Qt::AlignCenter);    // Add ImageWidget to the layout

    // Set the layout as the central widget's layout
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);


    // Connect the image load failure signal from ImageWidget to show an error message
    connect(imageWidget, &ImageWidget::imageLoadFailed, this, [] {
        QMessageBox::warning(nullptr, "Error", "Failed to load the image.");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}


