#include "image_dialog.h"
#include <QFileDialog>
#include <QString>

#include "../core/engines/thread_pool.h"

ImageDialog::ImageDialog(QWidget *parent) : QWidget(parent) {}

void ImageDialog::openFolder(ImageRepository *imageRepo, QWidget *parent) {
  QString folderPath = QFileDialog::getExistingDirectory(
      parent, "Select Folder", QDir::root().path(), QFileDialog::ShowDirsOnly);

    if (!folderPath.size())
      return;

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

    if (!fileName.size())
        return;

    imageRepo->load_image(fileName.toStdString());
}
