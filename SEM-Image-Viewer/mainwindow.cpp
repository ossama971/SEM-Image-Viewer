#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDir>
#include "opencv2/opencv.hpp"
#include "boost/filesystem.hpp"
#include <iostream>

using namespace cv;
namespace fs = boost::filesystem;
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    QDir appDir(QCoreApplication::applicationDirPath());
    appDir.cdUp();  // Move out of the MacOS folder
    appDir.cdUp();  // Move out of the Contents folder
    ui->setupUi(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_imporbtn_clicked()
{
    QDir appDir(QCoreApplication::applicationDirPath());
    for (int i =0 ;i < 4;i++)
        appDir.cdUp();  // Move out of the Build folder

    QString imagePath = appDir.absoluteFilePath("SEM-Image-Viewer/Utils/micro-electronic-sed.jpg");
    std::string file_path_str = imagePath.toStdString();

    cv::Mat image = cv::imread(file_path_str);
    cv::Mat rgb;
    cv::cvtColor(image, rgb, cv::COLOR_BGR2RGB);  // Convert BGR to RGB
    QImage qimg(rgb.data, rgb.cols, rgb.rows, rgb.step, QImage::Format_RGB888);
    QPixmap pixmap = QPixmap::fromImage(qimg);
    ui->mainViewer->setPixmap(pixmap.scaled(ui->mainViewer->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

