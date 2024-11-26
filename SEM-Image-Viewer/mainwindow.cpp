#include "core/engines/json_vsitor.h"
#include "core/engines/logger.h"
#include "core/engines/thread_pool.h"
#include "core/engines/workspace.h"
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

  leftSidebarWidget = new LeftSidebarWidget(this);
  leftSidebarWidget->setMinimumWidth(160);

  rightSidebarWidget = new RightSidebarWidget(this);
  rightSidebarWidget->setMinimumWidth(190);

  toolbarWidget = new ToolbarWidget(this);

  topMiddleWidget = new TopMiddleWidget(this);
  bottomMiddleWidget = new BottomMiddleWidget(this);
  miniGrid = new MiniGrid(this);
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
  // Open a folder browser to select the base directory
  QString baseDirectory = QFileDialog::getExistingDirectory(
      this, "Select Base Directory to Save Session");
  if (baseDirectory.isEmpty()) {
    // TODO: use logger instead of QMessageBox
    QMessageBox::warning(this, "No Directory Selected",
                         "You must select a directory to save the session.");
    return;
  }

  // Set default session folder name and JSON file name
  std::filesystem::path sessionFolderPath =
      std::filesystem::path(baseDirectory.toStdString());
  std::filesystem::path jsonFilePath = sessionFolderPath / "session.json";

  // Check if the session folder already exists
  if (std::filesystem::exists(jsonFilePath)) {
    // TODO: use logger instead of QMessageBox
    QMessageBox::warning(
        this, "Folder Exists",
        QString("The folder '%1' already exists. Please choose a different "
                "location or delete the existing folder.")
            .arg(QString::fromStdString(sessionFolderPath.string())));
    return;
  }

  try {
    // Log the save action
    int progressbarID = Logger::instance()->logMessageWithProgressBar(
        Logger::MessageTypes::info, Logger::MessageID::saving_session,
        Logger::MessageOption::with_path,
        {QString::fromStdString(jsonFilePath.string())},
        Workspace::Instance()
            ->getActiveSession()
            .getImageRepo()
            .getImagesCount());

    // Save the session using a thread pool task
    auto saveTask =
        post(ThreadPool::instance(),
             use_future([sessionFolderPath, jsonFilePath, progressbarID]() {
               JsonVisitor visitor(sessionFolderPath.string(),
                                   jsonFilePath.string(), progressbarID);
               Workspace::Instance()->getActiveSession().accept(visitor);
               visitor.write_json();
             }));
    saveTask.get();
    // TODO: use logger instead of QMessageBox
    QMessageBox::information(this, "Save Successful",
                             "Session has been successfully saved.");
    QApplication::quit();
  } catch (const std::exception &e) {
    // TODO: use logger instead of QMessageBox
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
