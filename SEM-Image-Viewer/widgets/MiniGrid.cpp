#include "MiniGrid.h"
#include <QVBoxLayout>
#include <QScrollBar>
#include <QDebug>

MiniGrid::MiniGrid(QWidget *parent) : QWidget(parent), imageDataModel(new ImageDataModel(this)) {
    listView = new QListView(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(listView);
    setLayout(layout);

    // Set a fixed height for the list view
    int rowHeight = 70;
    listView->setFixedHeight(rowHeight);

    // Automatically set up the model and initialize the grid
    setModel(imageDataModel);
    loadImages("C:/Users/nahel/OneDrive/Pictures/test"); // Replace with the appropriate path
}

void MiniGrid::setModel(ImageDataModel *model) {
    imageDataModel = model;
    listView->setModel(imageDataModel);
    initializeMiniGrid();
}

void MiniGrid::initializeMiniGrid() {
    int maxItemsPerRow = 4; // Adjust based on thumbnail width or requirement
    if (imageDataModel->rowCount() > maxItemsPerRow) {
        ImageDataModel *limitedModel = new ImageDataModel(this);
        for (int i = 0; i < maxItemsPerRow && i < imageDataModel->rowCount(); ++i) {
            limitedModel->appendImage(imageDataModel->getImageAt(i));
        }
        listView->setModel(limitedModel);
    } else {
        listView->setModel(imageDataModel);
    }

    listView->setViewMode(QListView::IconMode);
    listView->setSpacing(50);
    listView->setResizeMode(QListView::Adjust);
    listView->setFlow(QListView::LeftToRight);
    listView->setUniformItemSizes(true);
    listView->setGridSize(QSize(120, 120)); // Adjust grid size for one row

    // Disable scrolling entirely
    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

void MiniGrid::loadImages(const QString &path) {
    if (!imageDataModel->loadImagesFromPath(path)) {
        // Handle error if needed (e.g., log or show a message)
    }
}
