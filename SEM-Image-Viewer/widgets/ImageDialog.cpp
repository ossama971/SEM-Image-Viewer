#include "ImageDialog.h"
#include <QFileDialog>
#include <QThread>

ImageDialog::ImageDialog(QWidget* parent) : QWidget(parent) {
}

void ImageDialog::openFolder(ImageRepository* imageRepo, QWidget* parent) {
    QString folderPath = QFileDialog::getExistingDirectory(
        parent,               // Parent widget
        "Select Folder",       // Dialog title
        QDir::root().path(),                    // Initial directory (empty means default)
        QFileDialog::ShowDirsOnly // Show only directories, not files
        );

    QThread* thread = new QThread;

    connect(thread, &QThread::started, [imageRepo, folderPath]() {
        imageRepo->load_directory(folderPath.toStdString());
    });

    thread->start();
}


void ImageDialog::openFile(ImageRepository* imageRepo, QWidget* parent) {
    QString fileName = QFileDialog::getOpenFileName(
        parent,               // Parent widget
        "Open File",           // Dialog title
        QDir::root().path(),  // Initial directory (empty means default)
        "All Files (*.*);;Text Files (*.txt);;Image Files (*.png *.jpg)"  // Filters
        );

    imageRepo->load_image(fileName.toStdString());
}
