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


    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(mainSplitter);
    layout->setContentsMargins(0, 0, 0, 0);
    centralWidget->setLayout(layout);


    menuBarWidget = new MenuBarWidget(this);


    connect(menuBarWidget, &MenuBarWidget::showLeftSidebarClicked, this, &MainWindow::onShowLeftSidebarClicked);
    connect(menuBarWidget, &MenuBarWidget::showRightSidebarClicked, this, &MainWindow::onShowRightSidebarClicked);
    connect(menuBarWidget, &MenuBarWidget::showLoggerClicked, this, &MainWindow::onShowLoggerClicked);
    connect(menuBarWidget, &MenuBarWidget::showImageClicked, this, &MainWindow::onShowImageClicked);
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
    // Your code to save changes here

    // JsonVisitor visitor;
    // Workspace::Instance()->getActiveSession().accept(visitor);
    // visitor.write_json("session.json");

    QMessageBox::information(this, "Save", "Changes have been saved.");
    QApplication::quit();  // Exit after saving
}


MainWindow::~MainWindow()
{
    delete ui;
}



