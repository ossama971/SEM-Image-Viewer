#include "ImageDialog.h"
#include <QFileDialog>

QString ImageDialog::openFolder(ImageRepository* imageRepo, QWidget* parent) {
    QString folderPath = QFileDialog::getExistingDirectory(
        parent,               // Parent widget
        "Select Folder",       // Dialog title
        QDir::root().path(),                    // Initial directory (empty means default)
        QFileDialog::ShowDirsOnly // Show only directories, not files
        );

    if (imageRepo->load_directory(folderPath.toStdString()))
        return folderPath;
    return QString();
}

QString ImageDialog::openFile(ImageRepository* imageRepo, QWidget* parent) {
    QString fileName = QFileDialog::getOpenFileName(
        parent,               // Parent widget
        "Open File",           // Dialog title
        QDir::root().path(),  // Initial directory (empty means default)
        "All Files (*.*);;Text Files (*.txt);;Image Files (*.png *.jpg)"  // Filters
        );

    if (imageRepo->load_image(fileName.toStdString()))
        return fileName;
    return QString();
}
