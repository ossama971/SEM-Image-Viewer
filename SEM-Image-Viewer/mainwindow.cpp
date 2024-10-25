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

    // Create a layout to hold the button and the ImageWidget
    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(ui->imporbtn, 0, Qt::AlignHCenter); // Add the button to the layout
    layout->addWidget(imageWidget, 0, Qt::AlignCenter);    // Add ImageWidget to the layout

    // Set the layout as the central widget's layout
    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // Connect the button click signal to the imporbtn_clicked slot
    connect(ui->imporbtn, &QPushButton::clicked, this, &MainWindow::imporbtn_clicked);

    // Connect the image load failure signal from ImageWidget to show an error message
    connect(imageWidget, &ImageWidget::imageLoadFailed, this, [] {
        QMessageBox::warning(nullptr, "Error", "Failed to load the image.");
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::imporbtn_clicked()
{
    imageWidget->loadAndDisplayImage("Put absolute path to any photo here");
}
