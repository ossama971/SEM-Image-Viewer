#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include "widgets/SaveSessionDialog.h"
#include "core/engines/JsonVisitor.h"
#include "core/engines/Workspace.h"
#include "core/engines/ThreadPool.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    // ui->setupUi(this);

    // resize(1000, 800);  // Adjust this size as needed
    this->setWindowState(Qt::WindowFullScreen);

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
    horizontalSplitter->setStretchFactor(0, 3);  // topMiddleWidget takes more space
    horizontalSplitter->setStretchFactor(1, 1);  // miniGrid remains small
    horizontalSplitter->setStretchFactor(2, 1);
    horizontalSplitter->setSizes({400, 110, 250});

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
    connect(leftSidebarWidget, &LeftSidebarWidget::onVisibilityChange, menuBarWidget, &MenuBarWidget::onLeftSidebarViewChanged);
    connect(rightSidebarWidget, &RightSidebarWidget::onVisibilityChange, menuBarWidget, &MenuBarWidget::onRightSidebarViewChanged);
    connect(topMiddleWidget, &TopMiddleWidget::onVisibilityChange, menuBarWidget, &MenuBarWidget::onImageViewChanged);
    connect(bottomMiddleWidget, &BottomMiddleWidget::onVisibilityChange, menuBarWidget, &MenuBarWidget::onLoggerViewChanged);
    connect(menuBarWidget, &MenuBarWidget::showLeftSidebarClicked, this, &MainWindow::onShowLeftSidebarClicked);
    connect(menuBarWidget, &MenuBarWidget::showRightSidebarClicked, this, &MainWindow::onShowRightSidebarClicked);
    connect(menuBarWidget, &MenuBarWidget::showLoggerClicked, this, &MainWindow::onShowLoggerClicked);
    connect(menuBarWidget, &MenuBarWidget::showImageClicked, this, &MainWindow::onShowImageClicked);
    // connect(menuBarWidget, &MenuBarWidget::exportStarted, rightSidebarWidget, &RightSidebarWidget::initializeProgress);
    // connect(menuBarWidget, &MenuBarWidget::exportProgressUpdated, rightSidebarWidget, &RightSidebarWidget::updateProgress);
    // connect(menuBarWidget, &MenuBarWidget::exportFinished, rightSidebarWidget, &RightSidebarWidget::hideProgressBar);
    connect(menuBarWidget, &MenuBarWidget::themeToggled, this, &MainWindow::applyTheme);

    // Connecting signals related to Toolbar
    HistoryWidget* historyWidget = rightSidebarWidget->findChild<HistoryWidget*>();
    // Connecting signals sent from Toolbar to History
    connect(toolbarWidget, &ToolbarWidget::undoTriggered, historyWidget, &HistoryWidget::undoAction);
    connect(toolbarWidget, &ToolbarWidget::redoTriggered, historyWidget, &HistoryWidget::redoAction);
    // Connecting signals sent from Toolbar to MenuBar
    connect(toolbarWidget, &ToolbarWidget::saveButtonClicked, menuBarWidget,[this]() {
        menuBarWidget->exportSelectedImage("*.jpg");});
    // Connecting signals sent from Toolbar to show/hide Logger
    connect(toolbarWidget, &ToolbarWidget::minimizeLoggerClicked, this, &MainWindow::onShowLoggerClicked);
    // Connecting signals sent from Toolbar to show/hide Minigrid
    connect(toolbarWidget, &ToolbarWidget::minimizeToolbarClicked, this, &MainWindow::showMiniGridClicked);
    connect(toolbarWidget->imageViewButton, &QToolButton::clicked, topMiddleWidget, &TopMiddleWidget::onimageViewButtonClicked);
    connect(toolbarWidget->diffViewButton, &QToolButton::clicked, topMiddleWidget, &TopMiddleWidget::ondiffViewButtonClicked);
    connect(toolbarWidget->gridViewButton, &QToolButton::clicked, topMiddleWidget, &TopMiddleWidget::ongridViewButtonClicked);

    connect(menuBarWidget,&MenuBarWidget::undoChecked,historyWidget,&HistoryWidget::undoAction);
    connect(menuBarWidget,&MenuBarWidget::redoChecked,historyWidget,&HistoryWidget::redoAction);
    // Manually setting cursor for splitter handles
    for (int i = 0; i < finalSplitter->count(); ++i) {
        QSplitterHandle *handle = finalSplitter->handle(i);
        if (handle) {
            handle->setCursor(Qt::ArrowCursor);  // Disable resizing cursor
        }
    }
    for (int i = 0; i < middleSplitter->count(); ++i) {
        QSplitterHandle *handle = middleSplitter->handle(i);
        if (handle) {
            handle->setCursor(Qt::ArrowCursor);
        }
    }

    ImageWidget *imageWidget= topMiddleWidget->findChild<ImageWidget*>();
    HeatMapWidget *heatmapWidget = rightSidebarWidget->findChild<HeatMapWidget*>();
    connect(heatmapWidget,&HeatMapWidget::applyHeatMap,imageWidget,&ImageWidget::handleHeatmap);
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
    if (Workspace::Instance()->getActiveSession().getImageRepo().getHasUnsavedChanges()){

        ExitDialogWidget exitDialog(this);

        connect(&exitDialog, &ExitDialogWidget::saveRequested, this, &MainWindow::onSaveChangesClicked);
        connect(&exitDialog, &ExitDialogWidget::dontSaveRequested, this, &QApplication::quit);

        if (exitDialog.exec() == QDialog::Rejected) {
            event->ignore();  // Keep the application open if the user clicked "Cancel"
        } else {
            event->accept();  // Close the application if the user clicked "Save" or "Don't Save"
        }
    }
}

void MainWindow::onSaveChangesClicked() {
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

        auto saveTask = post(ThreadPool::instance(), use_future([directoryPath, jsonFilePath, progressbarID]() {
            try {
                JsonVisitor visitor(directoryPath.string(), jsonFilePath.string(), progressbarID);
                Workspace::Instance()->getActiveSession().accept(visitor);
                visitor.write_json();
            } catch (const std::exception &e) {
                qDebug() << "Failed to save session: " << e.what();
                throw;
            }
        }));

        try {
            saveTask.get();
            QMessageBox::information(this, "Save", "Changes have been saved.");
            ThreadPool::instance().join();
            QApplication::quit();
            qDebug() << "Save successful. Exiting application.";
        } catch (const std::exception &e) {
            QMessageBox::critical(this, "Save Error", QString("Failed to save changes: %1").arg(e.what()));
        }
    }
}

void MainWindow::applyTheme() {
    static bool isDarkMode = false;

    QString styleFile = isDarkMode ? ":/styles/light-mode.qss" : ":/styles/dark-mode.qss";

    QFile file(styleFile);
    if (file.open(QFile::ReadOnly)) {
        QString stylesheet = QLatin1String(file.readAll());
        qApp->setStyleSheet(stylesheet);
        file.close();
    }

    isDarkMode = !isDarkMode;
}

MainWindow::~MainWindow()
{
    delete ui;
}



