#include "../core/engines/json_vsitor.h"
#include "../core/engines/logger.h"
#include "../core/engines/thread_pool.h"
#include "../core/engines/workspace.h"
#include "../core/utils.h"
#include "image_dialog.h"
#include "menu_bar_widget.h"
#include <QApplication>
#include <QFileDialog>
#include <QImage>
#include <QMenu>
#include <QMessageBox>
#include <boost/format.hpp>

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
  connect(closeAllAction, &QAction::triggered, this, &MenuBarWidget::closeAll);

  connect(JPGAllAction, &QAction::triggered, this,
          [=]() { exportImages("jpg"); });
  connect(PNGAllAction, &QAction::triggered, this,
          [=]() { exportImages("png"); });
  connect(BMPAllAction, &QAction::triggered, this,
          [=]() { exportImages("bmp"); });

  // selected image
  connect(JPGAction, &QAction::triggered, this,
          [=]() { exportSelectedImage("jpg"); });
  connect(PNGAction, &QAction::triggered, this,
          [=]() { exportSelectedImage("png"); });
  connect(BMPAction, &QAction::triggered, this,
          [=]() { exportSelectedImage("bmp"); });

  connect(saveSessionAction, &QAction::triggered, this,
          [=]() { saveSession(); });
  connect(loadSessionAction, &QAction::triggered, this,
          [=]() { loadSession(); });
}

void MenuBarWidget::exportSelectedImage(const QString &format) {
  const auto image = Workspace::Instance()
                         ->getActiveSession()
                         .getImageRepo()
                         .cloneSelectedImage();

  if (image == nullptr) {
    // TODO: use logger to inform user that there is no image to export
    return;
  }

  const std::string originalFileName = image->getPath().filename().string();
  const size_t lastDot = originalFileName.find_last_of('.');
  const std::string baseFileName = (lastDot != std::string::npos)
                                       ? originalFileName.substr(0, lastDot)
                                       : originalFileName;

  QString saveFilePath = QFileDialog::getSaveFileName(
      this, tr("Save Image File"), QString::fromStdString(baseFileName),
      tr("Images (*.%1)").arg(format));

  if (saveFilePath.isEmpty()) {
    // User canceled the dialog
    return;
  }

  if (!saveFilePath.endsWith("." + format, Qt::CaseInsensitive)) {
    saveFilePath += "." + format;
  }

  const cv::Mat& matImg = image->getImageMat();

  // Save the image using OpenCV
  const std::string saveFileName = saveFilePath.toStdString();
  post(ThreadPool::instance(),
       std::packaged_task<void()>(
           [matImg, saveFileName]() {
            std::filesystem::path parentPath = std::filesystem::path(saveFileName).parent_path();
            if (!Utils::checkWritePermission(parentPath)) {
              Logger::instance()->logMessage(
                  Logger::MessageTypes::error, Logger::MessageID::insufficient_permissions,
                  Logger::MessageOption::with_path,
                  {QString::fromStdString(parentPath.string())});
              return;
            }
             if(cv::imwrite(saveFileName, matImg)) {
               Logger::instance()->logMessage(
                  Logger::MessageTypes::info, Logger::MessageID::exporting_images,
                  Logger::MessageOption::with_path, { QString::fromStdString(saveFileName) },
                  QString("file:///%1").arg(QString::fromStdString(saveFileName)));
               } else {
               Logger::instance()->logMessage(
                   Logger::MessageTypes::error, Logger::MessageID::exporting_error,
                   Logger::MessageOption::with_path,
                   {QString::fromStdString(saveFileName)});
               }
           }));
}

void MenuBarWidget::exportImages(const QString &format) {
  QString directoryPath = QFileDialog::getExistingDirectory(
      this, tr("Select Directory to Save Images"));

  if (directoryPath.isEmpty()) {
    return;
  }

  if(!Utils::checkWritePermission(std::filesystem::path(directoryPath.toStdString()))) {
    Logger::instance()->logMessage(
        Logger::MessageTypes::error, Logger::MessageID::insufficient_permissions,
        Logger::MessageOption::with_path,
        {directoryPath});
    return;
  }

  // TODO: is it better to clone the images in a seperate thread in case of a
  // large number of images?
  const auto sh_images = std::make_shared<std::vector<std::unique_ptr<Image>>>(
      Workspace::Instance()->getActiveSession().getImageRepo().cloneImages());
  const auto images_count = sh_images->size();

  int progressbarID = Logger::instance()->logMessageWithProgressBar(
      Logger::MessageTypes::info, Logger::MessageID::exporting_images,
      Logger::MessageOption::with_path, {directoryPath}, images_count,
      QString("file:///%1").arg(directoryPath));

  auto saveImagesSubset = [this, sh_images, directoryPath, format,
                           progressbarID](size_t start, size_t end) {
    for (size_t i = start; i < end; ++i) {
      const auto &image = (*sh_images)[i];
      if (!image) {
        // use logger to inform that there is an error/warn in exporting the
        // image
        continue;
      }
      std::string fileName = image->getPath().filename().string();
      const size_t lastDot = fileName.find_last_of('.');
      if (lastDot != std::string::npos) {
        fileName = fileName.substr(0, lastDot);
      }
      const cv::Mat matImg = image->readImageMat();
      const std::string numberedFileName =
          (boost::format("%1%/%2%.%3%") % directoryPath.toStdString() %
           fileName % format.toStdString())
              .str();
      cv::imwrite(numberedFileName, matImg);
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

  connect(undoAction, &QAction::triggered, this, &MenuBarWidget::undoChecked);
  connect(redoAction, &QAction::triggered, this, &MenuBarWidget::redoChecked);
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
  // Open a folder browser to select the base directory
  QString baseDirectory = QFileDialog::getExistingDirectory(this, "Select Base Directory to Save Session");
  if (baseDirectory.isEmpty()) {
    // TODO: use logger instead of QMessageBox
    return;
  }

  // Set default session folder name and JSON file name
  std::filesystem::path sessionFolderPath = std::filesystem::path(baseDirectory.toStdString());
  std::filesystem::path jsonFilePath = sessionFolderPath / "session.json";

  // Check if the session folder already exists
  if (std::filesystem::exists(jsonFilePath)) {
    Logger::instance()->logMessage(
    Logger::MessageTypes::warning, Logger::MessageID::file_already_exists,
    Logger::MessageOption::with_path,
    {QString::fromStdString(sessionFolderPath.string())});
    return;
  }

  if (!Utils::checkWritePermission(sessionFolderPath)) {
    Logger::instance()->logMessage(
        Logger::MessageTypes::error, Logger::MessageID::insufficient_permissions,
        Logger::MessageOption::with_path,
        {QString::fromStdString(sessionFolderPath.string())});
    return;
  }

  try {
    int progressbarID = Logger::instance()->logMessageWithProgressBar(
        Logger::MessageTypes::info, Logger::MessageID::saving_session,
        Logger::MessageOption::with_path,
        {QString::fromStdString(jsonFilePath.string())},
        Workspace::Instance()
            ->getActiveSession()
            .getImageRepo()
            .getImagesCount());

    post(ThreadPool::instance(),
        [sessionFolderPath, jsonFilePath, progressbarID]() {
           JsonVisitor visitor(sessionFolderPath.string(), jsonFilePath.string(), progressbarID, nullptr);
           Workspace::Instance()->getActiveSession().accept(visitor);
           visitor.write_json();
         });
  } catch (const std::exception &e) {
    Logger::instance()->logMessage(
        Logger::MessageTypes::error, Logger::MessageID::error_in_save,
        Logger::MessageOption::without_path,
        {});
  }
}

void MenuBarWidget::loadSession() {
  QString file = QFileDialog::getOpenFileName(this, "Select Session File",
                                              QString(), "JSON Files (*.json)");
  if (file.isEmpty()) {
    return; // User canceled
  }

  std::filesystem::path jsonFilePath =
      std::filesystem::path(file.toStdString());

  // Check if the selected file has a .json extension
  if (jsonFilePath.extension() != ".json") {
    // TODO: use logger
    return; // Invalid file type
  }

  try {
    post(ThreadPool::instance(), [jsonFilePath]() { Utils::loadSessionJson(jsonFilePath.string()); });

    // TODO: use logger
    return; // Load successful
  } catch (const std::exception &e) {
    // TODO: use logger
    return; // Load failed
  }
}
