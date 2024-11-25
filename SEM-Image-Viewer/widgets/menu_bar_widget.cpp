#include "../core/engines/json_vsitor.h"
#include "../core/engines/thread_pool.h"
#include "../core/engines/workspace.h"
#include "../core/engines/logger.h"
#include "../core/utils.h"
#include "image_dialog.h"
#include "load_session_dialog.h"
#include "save_session_dialog.h"
#include "menu_bar_widget.h"
#include <QMenu>
#include <QImage>
#include <QFileDialog>
#include <QApplication>


MenuBarWidget::MenuBarWidget(QWidget *parent) : QMenuBar(parent) {

  imageDialog = new ImageDialog(this);

  fileMenu();
  editMenu();
  viewMenu();
  optionsMenu();

  // make the menu bar expanded by default
  this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}

void MenuBarWidget::fileMenu() {

  QMenu *fileMenu = this->addMenu("File");
  QMenu *exportAllMenu = new QMenu("Export All", fileMenu);
  QMenu *exportMenu = new QMenu("Export", fileMenu); // selected image

  QAction *openImageAction = new QAction("Open Image", this);
  QAction *openFolderAction = new QAction("Open Folder", this);

  // selected image
  QAction *JPGAction = new QAction("JPG", this);
  QAction *PNGAction = new QAction("PNG", this);
  QAction *BMPAction = new QAction("BMP", this);

  QAction *saveSessionAction = new QAction("Save Session", this);
  QAction *loadSessionAction = new QAction("Load Session", this);
  QAction *closeAllAction = new QAction("Close All", this);

  QAction *JPGAllAction = new QAction("JPG", this);
  QAction *PNGAllAction = new QAction("PNG", this);
  QAction *BMPAllAction = new QAction("BMP", this);

  exportAllMenu->addAction(JPGAllAction);
  exportAllMenu->addAction(PNGAllAction);
  exportAllMenu->addAction(BMPAllAction);

  // selected image
  exportMenu->addAction(JPGAction);
  exportMenu->addAction(PNGAction);
  exportMenu->addAction(BMPAction);

  fileMenu->addAction(openImageAction);
  fileMenu->addAction(openFolderAction);
  fileMenu->addSeparator();
  fileMenu->addMenu(exportMenu);
  fileMenu->addMenu(exportAllMenu);
  fileMenu->addSeparator();
  fileMenu->addAction(saveSessionAction);
  fileMenu->addAction(loadSessionAction);
  fileMenu->addAction(closeAllAction);

  connect(openImageAction, &QAction::triggered, this, [=]() {
    imageDialog->openFile(
        &Workspace::Instance()->getActiveSession().getImageRepo(), this);
  });
  connect(openFolderAction, &QAction::triggered, this, [=]() {
    imageDialog->openFolder(
        &Workspace::Instance()->getActiveSession().getImageRepo(), this);
  });
  connect(closeAllAction, &QAction::triggered, this,
          &MenuBarWidget::closeAll);

  connect(JPGAllAction, &QAction::triggered, this,
          [=]() { exportImages("jpg"); });
  connect(PNGAllAction, &QAction::triggered, this,
          [=]() { exportImages("png"); });
  connect(BMPAllAction, &QAction::triggered, this,
          [=]() { exportImages("bmp"); });

  // selected image
  connect(JPGAction, &QAction::triggered, this,
          [=]() { exportSelectedImage(".jpg"); });
  connect(PNGAction, &QAction::triggered, this,
          [=]() { exportSelectedImage(".png"); });
  connect(BMPAction, &QAction::triggered, this,
          [=]() { exportSelectedImage(".bmp"); });

  connect(saveSessionAction, &QAction::triggered, this,
          [=]() { saveSession(); });
  connect(loadSessionAction, &QAction::triggered, this,
          [=]() { loadSession(); });
}

void MenuBarWidget::exportSelectedImage(const QString &format) {
  Image *image =
      Workspace::Instance()->getActiveSession().getImageRepo().getImage();

  if(image==nullptr){
      //TODO: use logger to inform user that there is no image to export
      return;
  }
  string fileName = image->getPath().filename().string();
  size_t lastDot = fileName.find_last_of('.');
  QString baseName = QString::fromStdString(fileName);

  if (lastDot != string::npos) {
    fileName = fileName.substr(0, lastDot); // Remove the extension
  }

  QString directoryPath = QFileDialog::getSaveFileName(
      this, tr("Save Image File"),
      QString::fromStdString(image->getPath().filename().stem().string()),
      tr("Images (*.%1)").arg(format));

  QString baseFileName = QFileDialog::getSaveFileName(
      this, tr("Save Image File"), baseName, tr("Images (%1)").arg(format));

  if (!baseFileName.isEmpty()) {
    QFileInfo fileInfo(baseFileName);
    QString extension = fileInfo.completeSuffix();
    QString filePath = fileInfo.path();

    cv::Mat matImg = image->getImageMat();
    QImage qImg;
    if (matImg.type() == CV_8UC1) {
        // Grayscale image
        qImg = QImage(matImg.data, matImg.cols, matImg.rows, matImg.step, QImage::Format_Grayscale8);
    } else if (matImg.type() == CV_8UC3) {
        // RGB image (already 3 channels)
        qImg = QImage(matImg.data, matImg.cols, matImg.rows, matImg.step, QImage::Format_RGB888).rgbSwapped();
    } else {
        // Convert other types if necessary
        cv::Mat convertedImg;
        cv::cvtColor(matImg, convertedImg, cv::COLOR_BGR2RGB); // Assuming BGR format
        qImg = QImage(convertedImg.data, convertedImg.cols, convertedImg.rows, convertedImg.step, QImage::Format_RGB888);
    }

    QString numberedFileName =
        QString("%1/%2.%3").arg(filePath).arg(baseName).arg(extension);

    qImg.save(numberedFileName);
  }

  auto result = Utils::prepareImageForExport(
      image, QFileInfo(directoryPath).path(), format);
  if (!result) {
    //TODO: use logger to inform user that there is an error in exporting the image
    return;
  }

  const auto qImg = result->first;
  const auto numberedFileName = result->second;
  post(ThreadPool::instance(),
       std::packaged_task<void()>(
           [qImg, numberedFileName]() { qImg.save(numberedFileName); }));
}

void MenuBarWidget::exportImages(const QString &format) {
  QString directoryPath = QFileDialog::getExistingDirectory(
      this, tr("Select Directory to Save Images"));

  if (directoryPath.isEmpty()) {
    return;
  }

  auto images_count =
      Workspace::Instance()->getActiveSession().getImageRepo().getImagesCount();

  int progressbarID = Logger::instance()->logMessageWithProgressBar(
      Logger::MessageTypes::info,
      Logger::MessageID::exporting_images,
      Logger::MessageOption::with_path,
      { directoryPath },
      images_count,
      QString("file:///%1").arg(directoryPath)
  );

  auto saveImagesSubset = [this, directoryPath, format, progressbarID](size_t start,
                                                        size_t end) {
    for (size_t i = start; i < end; ++i) {
      auto image =
          Workspace::Instance()->getActiveSession().getImageRepo().getImage(i);
      auto result = Utils::prepareImageForExport(image, directoryPath, format);
      if (!result) {
        //TODO: use logger Failed to prepare image for export at index num i
        continue;
      }

      const auto &[qImg, numberedFileName] = *result;
      qImg.save(numberedFileName);
      Logger::instance()->updateProgressBar(progressbarID, 1);
    }
  };

  const std::size_t BATCH_SIZE = 17;

  for (std::size_t i = 0; i < images_count; i += BATCH_SIZE) {
    std::size_t startIdx = i;
    std::size_t endIdx = std::min(i + BATCH_SIZE, images_count);
    post(ThreadPool::instance(), std::packaged_task<void()>(std::bind(
                                     saveImagesSubset, startIdx, endIdx)));
  }
}



void MenuBarWidget::editMenu() {
  QMenu *editMenu = this->addMenu("Edit");

  QAction *undoAction = new QAction("Undo", this);
  QAction *redoAction = new QAction("Redo", this);

  editMenu->addAction(undoAction);
  editMenu->addAction(redoAction);

  connect(undoAction,&QAction::triggered,this,&MenuBarWidget::undoChecked);
  connect(redoAction,&QAction::triggered,this,&MenuBarWidget::redoChecked);
}

void MenuBarWidget::viewMenu() {
  QMenu *viewMenu = this->addMenu("View");

  showLeftSidebarAction = new QAction("Show Left Sidebar", this);
  showRightSidebarAction = new QAction("Show Right Sidebar", this);
  showLoggerAction = new QAction("Show Logger Sidebar", this);
  showImageAction = new QAction("Show Image Sidebar", this);

  showLeftSidebarAction->setCheckable(true);
  showRightSidebarAction->setCheckable(true);
  showLoggerAction->setCheckable(true);
  showImageAction->setCheckable(true);

  showLeftSidebarAction->setChecked(true);
  showRightSidebarAction->setChecked(true);
  showLoggerAction->setChecked(true);
  showImageAction->setChecked(true);

  connect(showLeftSidebarAction, &QAction::triggered, this,
          &MenuBarWidget::showLeftSidebarClicked);
  connect(showRightSidebarAction, &QAction::triggered, this,
          &MenuBarWidget::showRightSidebarClicked);
  connect(showImageAction, &QAction::triggered, this,
          &MenuBarWidget::showImageClicked);
  connect(showLoggerAction, &QAction::triggered, this,
          &MenuBarWidget::showLoggerClicked);

  viewMenu->addAction(showLeftSidebarAction);
  viewMenu->addAction(showRightSidebarAction);
  viewMenu->addAction(showLoggerAction);
  viewMenu->addAction(showImageAction);
}

void MenuBarWidget::optionsMenu() {
  QMenu *optionsMenu = this->addMenu("Options");

  darkModeAction = new QAction(this);
  darkModeAction->setText("Dark Mode");
  optionsMenu->addAction(darkModeAction);
  connect(darkModeAction, &QAction::triggered, this,
          &MenuBarWidget::onThemeActionTriggered);
}

void MenuBarWidget::onLeftSidebarViewChanged(bool state) {
  showLeftSidebarAction->setChecked(state);
}

void MenuBarWidget::onRightSidebarViewChanged(bool state) {
  showRightSidebarAction->setChecked(state);
}

void MenuBarWidget::onImageViewChanged(bool state) {
  showImageAction->setChecked(state);
}

void MenuBarWidget::onLoggerViewChanged(bool state) {
  showLoggerAction->setChecked(state);
}
void MenuBarWidget::onThemeActionTriggered() {
  isDarkMode = !isDarkMode;
  if (isDarkMode) {
    darkModeAction->setText("Light Mode");
  } else {
    darkModeAction->setText("Dark Mode");
  }
  emit themeToggled();
}

void MenuBarWidget::saveSession() {
  SaveSessionDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {

    auto directoryPath = dialog.getDirectoryPath();
    auto jsonFilePath = dialog.getJsonFilePath();

    int progressbarID = Logger::instance()->logMessageWithProgressBar(
        Logger::MessageTypes::info,
        Logger::MessageID::saving_session,
        Logger::MessageOption::without_path,
        { QString::fromStdString(jsonFilePath.string()) },
        Workspace::Instance()->getActiveSession().getImageRepo().getImagesCount()
    );

    post(ThreadPool::instance(), [directoryPath, jsonFilePath, progressbarID]() {
        JsonVisitor visitor(directoryPath.string(), jsonFilePath.string(), progressbarID);
        Workspace::Instance()->getActiveSession().accept(visitor);
        visitor.write_json();
    });
  }
}

void MenuBarWidget::loadSession() {
  LoadSessionDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted) {
    auto jsonFilePath = dialog.getJsonFilePath();

    post(ThreadPool::instance(), [jsonFilePath]() {
      Utils::loadSessionJson(jsonFilePath.string());
    });

  }
}
