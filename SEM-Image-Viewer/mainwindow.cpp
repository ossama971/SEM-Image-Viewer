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

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{

    // ui->setupUi(this);

    resize(1000, 600); // Adjust this size as needed

    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    leftSidebarWidget = new LeftSidebarWidget(this);

    // Connect the image load failure signal from ImageWidget to show an error message
    connect(imageWidget, &ImageWidget::imageLoadFailed, this, []
            { QMessageBox::warning(nullptr, "Error", "Failed to load the image."); });

    rightSidebarWidget = new RightSidebarWidget(this);

    topMiddleWidget = new TopMiddleWidget(this);

    bottomMiddleWidget = new BottomMiddleWidget(this);

    QSplitter *middleSplitter = new QSplitter(Qt::Vertical, this);
    middleSplitter->addWidget(topMiddleWidget);
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

    menuBarWidget = new MenuBarWidget();

    connect(menuBarWidget, &MenuBarWidget::showLeftSidebarClicked, this, &MainWindow::onShowLeftSidebarClicked);
    connect(menuBarWidget, &MenuBarWidget::showRightSidebarClicked, this, &MainWindow::onShowRightSidebarClicked);
    connect(menuBarWidget, &MenuBarWidget::showLoggerClicked, this, &MainWindow::onShowLoggerClicked);
    connect(menuBarWidget, &MenuBarWidget::showImageClicked, this, &MainWindow::onShowImageClicked);
}

void MainWindow::onShowLeftSidebarClicked(bool isChecked)
{
    qDebug("inside onShowLeftSidebarClicked");
    if (isChecked)
    {
        leftSidebarWidget->show();
    }
    else
    {
        leftSidebarWidget->hide();
    }
}

void MainWindow::onShowRightSidebarClicked(bool isChecked)
{
    qDebug("inside onShowRightSidebarClicked");
    if (isChecked)
    {
        rightSidebarWidget->show();
    }
    else
    {
        rightSidebarWidget->hide();
    }
}

void MainWindow::onShowLoggerClicked(bool isChecked)
{
    qDebug("inside onShowLoggerClicked");
    if (isChecked)
    {
        bottomMiddleWidget->show();
    }
    else
    {
        bottomMiddleWidget->hide();
    }
}

void MainWindow::onShowImageClicked(bool isChecked)
{
    qDebug("inside onShowImageClicked");
    if (isChecked)
    {
        topMiddleWidget->show();
    }
    else
    {
        topMiddleWidget->hide();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
