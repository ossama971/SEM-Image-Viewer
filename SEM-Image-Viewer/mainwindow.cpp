#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/MiniGrid.h"

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

#include "core/engines/JsonVisitor.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    // ui->setupUi(this);

    resize(1000, 600);  // Adjust this size as needed

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    leftSidebarWidget = new LeftSidebarWidget(this);

    rightSidebarWidget = new RightSidebarWidget(this);

    topMiddleWidget = new TopMiddleWidget(this);

    bottomMiddleWidget = new BottomMiddleWidget(this);

    viewController = new WidgetViewController(leftSidebarWidget, rightSidebarWidget, topMiddleWidget, bottomMiddleWidget, this);

    leftSidebarWidget->setViewController(viewController);
    rightSidebarWidget->setViewController(viewController);
    topMiddleWidget->setViewController(viewController);
    bottomMiddleWidget->setViewController(viewController);

    // Create the MiniGrid instance
    MiniGrid *miniGrid = new MiniGrid(this);

    QSplitter *middleSplitter = new QSplitter(Qt::Vertical, this);
    middleSplitter->addWidget(topMiddleWidget);
    middleSplitter->addWidget(miniGrid); // Add MiniGrid here
    middleSplitter->addWidget(bottomMiddleWidget);
    middleSplitter->setStretchFactor(0, 1);
    middleSplitter->setStretchFactor(1, 1);


    QSplitter *mainSplitter = new QSplitter(Qt::Horizontal, this);
    mainSplitter->addWidget(leftSidebarWidget);
    mainSplitter->addWidget(middleSplitter);
    mainSplitter->addWidget(rightSidebarWidget);
    mainSplitter->setStretchFactor(0, 1);
    mainSplitter->setStretchFactor(1, 3);
    mainSplitter->setStretchFactor(2, 1);

    menuBarWidget = new MenuBarWidget(viewController, this);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(menuBarWidget);
    layout->addWidget(mainSplitter);
    layout->setContentsMargins(0, 0, 0, 0);
    centralWidget->setLayout(layout);

    //connect(menuBarWidget, &MenuBarWidget::showLeftSidebarClicked, this, &MainWindow::onShowLeftSidebarClicked);
    //connect(menuBarWidget, &MenuBarWidget::showRightSidebarClicked, this, &MainWindow::onShowRightSidebarClicked);
    //connect(menuBarWidget, &MenuBarWidget::showLoggerClicked, this, &MainWindow::onShowLoggerClicked);
    //connect(menuBarWidget, &MenuBarWidget::showImageClicked, this, &MainWindow::onShowImageClicked);
    connect(menuBarWidget, &MenuBarWidget::exportStarted, rightSidebarWidget, &RightSidebarWidget::initializeProgress);
    connect(menuBarWidget, &MenuBarWidget::exportProgressUpdated, rightSidebarWidget, &RightSidebarWidget::updateProgress);
    connect(menuBarWidget, &MenuBarWidget::exportFinished, rightSidebarWidget, &RightSidebarWidget::hideProgressBar);
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
    SaveDialogWidget saveDialog(this);

    connect(&saveDialog, &SaveDialogWidget::saveRequested, this, &MainWindow::onSaveChangesClicked);
    connect(&saveDialog, &SaveDialogWidget::dontSaveRequested, this, &QApplication::quit);

    if (saveDialog.exec() == QDialog::Rejected) {
        event->ignore();  // Keep the application open if the user clicked "Cancel"
    } else {
        event->accept();  // Close the application if the user clicked "Save" or "Don't Save"
    }
}

void MainWindow::onSaveChangesClicked() {
    // If a thread is already running, return early (optional)
    if (saveThread && saveThread->isRunning()) {
        QMessageBox::information(this, "Save", "A save operation is already in progress.");
        return;
    }

    // Create the QThread and worker objects
    saveThread = new QThread(this);  // Store the thread as a member variable
    QObject *worker = new QObject();
    worker->moveToThread(saveThread);

    // Start the save operation in the worker thread
    connect(saveThread, &QThread::started, worker, [worker]() {
        JsonVisitor visitor;
        Workspace::Instance()->getActiveSession().accept(visitor);
        visitor.write_json("session.json");
    });

    // When worker finishes, quit the thread and delete worker
    connect(worker, &QObject::destroyed, saveThread, &QThread::quit);
    connect(saveThread, &QThread::finished, saveThread, &QThread::deleteLater);
    connect(saveThread, &QThread::finished, worker, &QObject::deleteLater);

    // When the thread finishes, show a message and quit the application
    connect(saveThread, &QThread::finished, this, [this]() {
        QMessageBox::information(nullptr, "Save", "Changes have been saved.");
        QApplication::quit();  // Exit the application after saving
    });

    // Start the thread
    saveThread->start();
}


MainWindow::~MainWindow()
{
    // If the saveThread is running, quit and wait for it to finish
    if (saveThread && saveThread->isRunning()) {
        saveThread->quit();  // Request the thread to quit
        saveThread->wait();  // Wait for the thread to finish
    }
    
    delete ui;  // Call the default destructor to clean up the UI
}



