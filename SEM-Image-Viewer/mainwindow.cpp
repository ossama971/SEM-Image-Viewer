#include "core/engines/json_vsitor.h"
#include "core/engines/logger.h"
#include "core/engines/thread_pool.h"
#include "core/engines/workspace.h"
#include "models/image_data_model.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QApplication>
#include <QMainWindow>
#include <QMessageBox>
#include <QSplitter>
#include <QVBoxLayout>
#include <QWidget>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {

  // ui->setupUi(this);

  this->setWindowState(Qt::WindowMaximized);

  QWidget *centralWidget = new QWidget(this);
  setCentralWidget(centralWidget);

  gridDataModel = new ImageDataModel(this);

  leftSidebarWidget = new LeftSidebarWidget(this);
  leftSidebarWidget->setMinimumWidth(160);

  rightSidebarWidget = new RightSidebarWidget(this);
  rightSidebarWidget->setMinimumWidth(210);

  toolbarWidget = new ToolbarWidget(this);

  topMiddleWidget = new TopMiddleWidget(this, toolbarWidget, gridDataModel);
  bottomMiddleWidget = new BottomMiddleWidget(this);

  miniGrid = new MiniGrid(this, gridDataModel);
  miniGrid->setMinimumHeight(90);
  miniGrid->setMaximumHeight(120);

  menuBarWidget = new MenuBarWidget(this);

  // Fixed splitter below the menu bar
  QWidget *fixedSplitter = new QWidget(this);
  fixedSplitter->setFixedHeight(0);
  fixedSplitter->setCursor(Qt::ArrowCursor);

  QSplitter *horizontalSplitter = new QSplitter(Qt::Vertical, this);
  horizontalSplitter->addWidget(topMiddleWidget);
  horizontalSplitter->addWidget(miniGrid);
  horizontalSplitter->addWidget(bottomMiddleWidget);
  horizontalSplitter->setStretchFactor(0,
                                       3); // topMiddleWidget takes more space
  horizontalSplitter->setStretchFactor(1, 1); // miniGrid remains small
  horizontalSplitter->setStretchFactor(2, 1);
  horizontalSplitter->setSizes({450, 110, 200});
  QSplitter *middleSplitter = new QSplitter(Qt::Vertical, this);
  middleSplitter->addWidget(toolbarWidget);
  middleSplitter->addWidget(fixedSplitter);
  middleSplitter->addWidget(horizontalSplitter);
  QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
  mainSplitter->addWidget(leftSidebarWidget);
  mainSplitter->addWidget(middleSplitter);
  mainSplitter->addWidget(rightSidebarWidget);
  mainSplitter->setStretchFactor(0, 1);
  mainSplitter->setStretchFactor(1, 3);
  mainSplitter->setStretchFactor(2, 1);
  QSplitter *finalSplitter = new QSplitter(Qt::Vertical, this);
  finalSplitter->addWidget(fixedSplitter);
  finalSplitter->addWidget(mainSplitter);
  QVBoxLayout *layout = new QVBoxLayout();
  layout->addWidget(finalSplitter);
  layout->setContentsMargins(0, 0, 0, 0);
  centralWidget->setLayout(layout);

  // Connecting Signals related to MenuBar
  setMenuBar(menuBarWidget);
  connect(leftSidebarWidget, &LeftSidebarWidget::onVisibilityChange,
          menuBarWidget, &MenuBarWidget::onLeftSidebarViewChanged);
  connect(rightSidebarWidget, &RightSidebarWidget::onVisibilityChange,
          menuBarWidget, &MenuBarWidget::onRightSidebarViewChanged);
  connect(topMiddleWidget, &TopMiddleWidget::onVisibilityChange, menuBarWidget,
          &MenuBarWidget::onImageViewChanged);
  connect(bottomMiddleWidget, &BottomMiddleWidget::onVisibilityChange,
          menuBarWidget, &MenuBarWidget::onLoggerViewChanged);
  connect(menuBarWidget, &MenuBarWidget::showLeftSidebarClicked, this,
          &MainWindow::onShowLeftSidebarClicked);
  connect(menuBarWidget, &MenuBarWidget::showRightSidebarClicked, this,
          &MainWindow::onShowRightSidebarClicked);
  connect(menuBarWidget, &MenuBarWidget::showLoggerClicked, this,
          &MainWindow::onShowLoggerClicked);
  connect(menuBarWidget, &MenuBarWidget::showImageClicked, this,
          &MainWindow::onShowImageClicked);
  // connect(menuBarWidget, &MenuBarWidget::exportStarted, rightSidebarWidget,
  // &RightSidebarWidget::initializeProgress); connect(menuBarWidget,
  // &MenuBarWidget::exportProgressUpdated, rightSidebarWidget,
  // &RightSidebarWidget::updateProgress); connect(menuBarWidget,
  // &MenuBarWidget::exportFinished, rightSidebarWidget,
  // &RightSidebarWidget::hideProgressBar);
  connect(menuBarWidget, &MenuBarWidget::themeToggled, this,
          &MainWindow::applyTheme);

  // Connecting signals related to Toolbar
  HistoryWidget *historyWidget =
      rightSidebarWidget->findChild<HistoryWidget *>();
  // Connecting signals sent from Toolbar to History
  connect(toolbarWidget, &ToolbarWidget::undoTriggered, historyWidget,
          &HistoryWidget::undoAction);
  connect(toolbarWidget, &ToolbarWidget::redoTriggered, historyWidget,
          &HistoryWidget::redoAction);
  // Connecting signals sent from Toolbar to MenuBar
  // TODO: fix this to be as the extension of the image
  connect(toolbarWidget, &ToolbarWidget::saveButtonClicked, menuBarWidget,
          [this]() { menuBarWidget->exportSelectedImage("jpg"); });
  // Connecting signals sent from Toolbar to show/hide Logger
  connect(toolbarWidget, &ToolbarWidget::minimizeLoggerClicked, this,
          &MainWindow::onShowLoggerClicked);
  // Connecting signals sent from Toolbar to show/hide Minigrid
  connect(toolbarWidget, &ToolbarWidget::minimizeToolbarClicked, this,
          &MainWindow::showMiniGridClicked);
  connect(toolbarWidget->imageViewButton, &QToolButton::clicked,
          topMiddleWidget, &TopMiddleWidget::onimageViewButtonClicked);
  connect(toolbarWidget->diffViewButton, &QToolButton::clicked, topMiddleWidget,
          &TopMiddleWidget::ondiffViewButtonClicked);
  connect(toolbarWidget->gridViewButton, &QToolButton::clicked, topMiddleWidget,
          &TopMiddleWidget::ongridViewButtonClicked);

  connect(menuBarWidget, &MenuBarWidget::undoChecked, historyWidget,
          &HistoryWidget::undoAction);
  connect(menuBarWidget, &MenuBarWidget::redoChecked, historyWidget,
          &HistoryWidget::redoAction);

  connect(menuBarWidget, &MenuBarWidget::closeAll, this,
          &MainWindow::onCloseAllClicked);
  // Manually setting cursor for splitter handles
  for (int i = 0; i < finalSplitter->count(); ++i) {
    QSplitterHandle *handle = finalSplitter->handle(i);
    if (handle) {
      handle->setCursor(Qt::ArrowCursor); // Disable resizing cursor
    }
  }
  for (int i = 0; i < middleSplitter->count(); ++i) {
    QSplitterHandle *handle = middleSplitter->handle(i);
    if (handle) {
      handle->setCursor(Qt::ArrowCursor);
    }
  }

  ImageWidget *imageWidget = topMiddleWidget->findChild<ImageWidget *>();
  HeatMapWidget *heatmapWidget =
      rightSidebarWidget->findChild<HeatMapWidget *>();
  connect(heatmapWidget, &HeatMapWidget::applyHeatMap, imageWidget,
          &ImageWidget::handleHeatmap);
}

void MainWindow::onShowLeftSidebarClicked(bool isChecked) {
  if (isChecked) {
    leftSidebarWidget->show();
  } else {
    leftSidebarWidget->hide();
  }
}

void MainWindow::onShowRightSidebarClicked(bool isChecked) {
  if (isChecked) {
    rightSidebarWidget->show();
  } else {
    rightSidebarWidget->hide();
  }
}

void MainWindow::showMiniGridClicked(bool isChecked) {
  if (isChecked) {
    miniGrid->show();
  } else {
    miniGrid->hide();
  }
}

void MainWindow::onShowLoggerClicked(bool isChecked) {
  if (isChecked) {
    bottomMiddleWidget->show();
  } else {
    bottomMiddleWidget->hide();
  }
}

void MainWindow::onShowImageClicked(bool isChecked) {
  if (isChecked) {
    topMiddleWidget->show();

  } else {
    topMiddleWidget->hide();
  }
}

void MainWindow::closeEvent(QCloseEvent *event) {
  if (Workspace::Instance()
          ->getActiveSession()
          .getImageRepo()
          .getHasUnsavedChanges()) {

    ExitDialogWidget exitDialog(this);

    connect(&exitDialog, &ExitDialogWidget::saveRequested, this,
            &MainWindow::onSaveChangesClicked);
    connect(&exitDialog, &ExitDialogWidget::dontSaveRequested, this,
            &QApplication::quit);

    if (exitDialog.exec() == QDialog::Rejected) {
      event->ignore(); // Keep the application open if the user clicked "Cancel"
    } else {
      event->accept(); // Close the application if the user clicked "Save" or
                       // "Don't Save"
    }
  }
}

void MainWindow::onCloseAllClicked() { this->close(); }

void MainWindow::onSaveChangesClicked() {
    QString baseDirectory = QFileDialog::getExistingDirectory(this, "Select Base Directory to Save Session");
    if (baseDirectory.isEmpty()) {
        return;
    }

    std::filesystem::path sessionFolderPath = std::filesystem::path(baseDirectory.toStdString());
    std::filesystem::path jsonFilePath = sessionFolderPath / "session.json";

    if (std::filesystem::exists(jsonFilePath)) {
        Logger::instance()->logMessage(
            Logger::MessageTypes::warning, Logger::MessageID::file_already_exists,
            Logger::MessageOption::with_path,
            {QString::fromStdString(sessionFolderPath.string())});
        return;
    }

    try {
        setEnabled(false);

        progressBar = new QProgressBar(this);
        progressBar->setRange(0, Workspace::Instance()->getActiveSession().getImageRepo().getImagesCount());
        progressBar->setValue(0);
        progressBar->setTextVisible(true);
        progressBar->show();

        // Calculate the center position
        int windowWidth = this->width();
        int windowHeight = this->height();
        int progressBarWidth = 333;
        int progressBarHeight = 37;
        
        int x = (windowWidth - progressBarWidth) / 2;
        int y = (windowHeight - progressBarHeight) / 2;
        
        progressBar->setGeometry(x, y, progressBarWidth, progressBarHeight);

        post(ThreadPool::instance(), [sessionFolderPath, jsonFilePath, this]() {
            auto saveTask = post(ThreadPool::instance(), use_future([this, sessionFolderPath, jsonFilePath]() {
                auto progressCallback = [this](int progress) {
                    QMetaObject::invokeMethod(this, [this, progress]() {
                        int new_value = progressBar->value() + 1;
                        progressBar->setValue(new_value);
                    });
                };
                JsonVisitor visitor(sessionFolderPath.string(), jsonFilePath.string(), -100, progressCallback);
                Workspace::Instance()->getActiveSession().accept(visitor);
                visitor.write_json();
            }));
            saveTask.get();
            Workspace::Instance()->getActiveSession().getImageRepo().setHasUnsavedChanges(false);
            QMetaObject::invokeMethod(qApp, &QApplication::quit);
        });
    } catch (const std::exception &e) {
        Logger::instance()->logMessage(
            Logger::MessageTypes::error, Logger::MessageID::error_in_save,
            Logger::MessageOption::without_path,
            {});
    }
}

void MainWindow::applyTheme() {
  static bool isDarkMode = false;

  QString styleFile =
      isDarkMode ? ":/styles/light-mode.qss" : ":/styles/dark-mode.qss";

  QFile file(styleFile);
  if (file.open(QFile::ReadOnly)) {
    QString stylesheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(stylesheet);
    file.close();
  }

  isDarkMode = !isDarkMode;
}

MainWindow::~MainWindow() { delete ui; }
