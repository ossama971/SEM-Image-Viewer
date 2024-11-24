#include "../core/engines/JsonVisitor.h"
#include "../core/engines/ThreadPool.h"
#include "../core/engines/Workspace.h"
#include "../core/engines/Logger.h"
#include "../core/utils.h"

#include "ImageDialog.h"
#include "LoadSessionDialog.h"
#include "SaveSessionDialog.h"
#include "menubarwidget.h"

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

  connect(openImageAction, &QAction::triggered, this, [=]() {
    imageDialog->openFile(
        &Workspace::Instance()->getActiveSession().getImageRepo(), this);
  });
  connect(openFolderAction, &QAction::triggered, this, [=]() {
    imageDialog->openFolder(
        &Workspace::Instance()->getActiveSession().getImageRepo(), this);
  });

  connect(JPGAllAction, &QAction::triggered, this,
          [=]() { exportImages(".jpg"); });
  connect(PNGAllAction, &QAction::triggered, this,
          [=]() { exportImages(".png"); });
  connect(BMPAllAction, &QAction::triggered, this,
          [=]() { exportImages(".bmp"); });

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

void MenuBarWidget::exportSelectedImage(QString format) {
    qDebug("exporting  selected img");
  Image *image =
      Workspace::Instance()->getActiveSession().getImageRepo().getImage();



  if(image==nullptr){
      //TODO: use logger to inform user that there is no image to export
      qDebug("No selected image to export");
      return;
  }
  string fileName = image->getPath().filename().string();
  size_t lastDot = fileName.find_last_of('.');

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
    qDebug() << "Failed to prepare image for export.";
    return;
  }

  const auto qImg = result->first;
  const auto numberedFileName = result->second;
  post(ThreadPool::instance(),
       std::packaged_task<void()>(
           [qImg, numberedFileName]() { qImg.save(numberedFileName); }));
}

void MenuBarWidget::exportImages(const QString format) {
  QString directoryPath = QFileDialog::getExistingDirectory(
      this, tr("Select Directory to Save Images"));

  if (directoryPath.isEmpty()) {
    return;
  }

  auto images_count =
      Workspace::Instance()->getActiveSession().getImageRepo().getImagesCount();

  int progressbarID = Logger::instance()->logMessageWithProgressBar(
      Logger::MessageTypes::INFO,
      Logger::MessageID::EXPORTING_IMAGES,
      Logger::MessageOptian::WITH_DETAILS_AND_PATH,
      { directoryPath },
      images_count,
      "Loading .... ",
      QString("file:///%1").arg(directoryPath)
  );

  auto saveImagesSubset = [this, directoryPath, format, progressbarID](size_t start,
                                                        size_t end) {
    for (size_t i = start; i < end; ++i) {
      auto image =
          Workspace::Instance()->getActiveSession().getImageRepo().getImage(i);
      auto result = Utils::prepareImageForExport(image, directoryPath, format);
      if (!result) {
        qDebug() << "Failed to prepare image for export at index:" << i;
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
  QAction *saveAction = new QAction("Save", this);
  QAction *findAction = new QAction("Find In Files", this);

  editMenu->addAction(undoAction);
  editMenu->addAction(redoAction);
  editMenu->addAction(saveAction);
  editMenu->addSeparator();
  editMenu->addAction(findAction);

  connect(undoAction,&QAction::triggered,this,&MenuBarWidget::undoChecked);
  connect(redoAction,&QAction::triggered,this,&MenuBarWidget::redoChecked);
}

void MenuBarWidget::viewMenu() {
  QMenu *viewMenu = this->addMenu("View");

  explorerAction = new QAction("Explorer", this);
  heatMapAction = new QAction("HeatMap", this);
  intensityPlotAction = new QAction("Intensity Plot", this);
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

  // showLeftSidebarAction->setChecked(false);
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
  viewMenu->addSeparator();
  viewMenu->addAction(explorerAction);
  viewMenu->addSeparator();
  viewMenu->addAction(heatMapAction);
  viewMenu->addAction(intensityPlotAction);
}

void MenuBarWidget::optionsMenu() {
  QMenu *optionsMenu = this->addMenu("Options");
  QMenu *fontMenu = new QMenu("Font", optionsMenu);

  darkModeAction = new QAction(this);
  darkModeAction->setText("Dark Mode");
  optionsMenu->addAction(darkModeAction);
  optionsMenu->addMenu(fontMenu);
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
        Logger::MessageTypes::INFO,
        Logger::MessageID::SAVING_SESSION,
        Logger::MessageOptian::WITH_DETAILS_AND_PATH,
        { QString::fromStdString(jsonFilePath.string()) },
        Workspace::Instance()->getActiveSession().getImageRepo().getImagesCount(),
        QString("Saving Images Data to %1 .... ").arg(QString::fromStdString(directoryPath.string()))
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
