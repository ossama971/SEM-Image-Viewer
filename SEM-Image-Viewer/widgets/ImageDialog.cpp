#include "ImageDialog.h"
#include <QFileDialog>

#include "../core/engines/ThreadPool.h"

ImageDialog::ImageDialog(QWidget *parent) : QWidget(parent) {}

void ImageDialog::openFolder(ImageRepository *imageRepo, QWidget *parent) {
  QString folderPath = QFileDialog::getExistingDirectory(
      parent, "Select Folder", QDir::root().path(), QFileDialog::ShowDirsOnly);

  // It might be necessary to have this call in another thread espcially if the
  // loaded folder contains a lot of images
  post(ThreadPool::instance(), [imageRepo, folderPath]() {
    imageRepo->load_directory(folderPath.toStdString());
  });
}

void ImageDialog::openFile(ImageRepository *imageRepo, QWidget *parent) {
  QString fileName =
      QFileDialog::getOpenFileName(parent, "Open File", QDir::root().path(),

                                   "All Files (*.*);;Text Files (*.txt);;Image "
                                   "Files (*.png *.jpg)");

    imageRepo->load_image(fileName.toStdString());
    imageRepo->selectImage(fileName.toStdString());
}
