#include "MiniGrid.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollBar>

MiniGrid::MiniGrid(QWidget *parent) : QWidget(parent), imageDataModel(new ImageDataModel(this)) {
    listView = new QListView(this);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Initialize left and right buttons
    leftButton = new QPushButton("<",this);
    rightButton = new QPushButton(">",this);

    // Apply styles to buttons
    leftButton->setStyleSheet(R"(
        QPushButton {
            background-color: #333;
            border: none;
            border-radius: 10px;
            padding: 10px;
            width: 5px;
            height: 30px;
            color: white;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #555;
        }
    )");

    rightButton->setStyleSheet(R"(
        QPushButton {
            background-color: #333;
            border: none;
            border-radius: 10px;
            padding: 10px;
            width: 5px;
            height: 30px;
            color: white;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #555;
        }
    )");

    // Add buttons to buttonLayout
    buttonLayout->addWidget(leftButton);
    buttonLayout->addWidget(listView);
    buttonLayout->addWidget(rightButton);

    // Add buttonLayout to mainLayout
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    // Set a fixed height for the list view
    int rowHeight = 70;
    listView->setFixedHeight(rowHeight);

    // Connect buttons to slots
    connect(leftButton, &QPushButton::clicked, this, &MiniGrid::scrollLeft);
    connect(rightButton, &QPushButton::clicked, this, &MiniGrid::scrollRight);

    // set up the model and initialize the grid
    setModel(imageDataModel);
    loadImages("C:/Users/nahel/OneDrive/Pictures/test");
}

void MiniGrid::setModel(ImageDataModel *model) {
    imageDataModel = model;
    listView->setModel(imageDataModel);
    initializeMiniGrid();
}

void MiniGrid::initializeMiniGrid() {
    listView->setViewMode(QListView::IconMode);
    listView->setSpacing(50);
    listView->setResizeMode(QListView::Adjust);
    listView->setFlow(QListView::LeftToRight);
    listView->setUniformItemSizes(true);
    listView->setGridSize(QSize(120, 120));

    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listView->setWrapping(false);
}

void MiniGrid::scrollLeft() {
    int currentValue = listView->horizontalScrollBar()->value();
    int stepSize = listView->width();  // Scroll by the width of the view to shift by one
    listView->horizontalScrollBar()->setValue(currentValue - stepSize);
}

void MiniGrid::scrollRight() {
    int currentValue = listView->horizontalScrollBar()->value();
    int stepSize = listView->width();  // Scroll by the width of the view to shift by one
    listView->horizontalScrollBar()->setValue(currentValue + stepSize);
}


void MiniGrid::loadImages(const QString &path) {
    if (!imageDataModel->loadImagesFromPath(path)) {
        // Handle error (e.g., log or show a message)
    }
}
