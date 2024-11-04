#include "GridView.h"
#include <QVBoxLayout>
#include <QHeaderView>

GridView::GridView(QWidget *parent) : QWidget(parent), imageDataModel(new ImageDataModel(this)) {
    listView = new QListView(this);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(listView);
    setLayout(layout);

    // Set up the model and load images automatically
    setModel(imageDataModel);
    loadImages("C:/Users/nahel/OneDrive/Pictures/test"); // Replace with the appropriate path
}

void GridView::setModel(ImageDataModel *model) {
    imageDataModel = model;
    listView->setModel(imageDataModel);
    initializeGrid();
}

void GridView::initializeGrid() {
    listView->setContentsMargins(0, 100, 0, 0);
    listView->setViewMode(QListView::IconMode);
    listView->setSpacing(50);
    listView->setResizeMode(QListView::Adjust);
    listView->setFlow(QListView::LeftToRight);
    listView->setUniformItemSizes(true);
    listView->setGridSize(QSize(120, 120));
}

void GridView::loadImages(const QString &path) {
    if (!imageDataModel->loadImagesFromPath(path)) {
        // Handle error
    }
}
