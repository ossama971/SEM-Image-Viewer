#include "GridView.h"
#include <QVBoxLayout>
#include <QHeaderView>
#include "../core/engines/Workspace.h"
#include "ThumbnailDelegate.h"

GridView::GridView(QWidget *parent) : QWidget(parent), imageDataModel(new ImageDataModel(this)) {
    listView = new QListView(this);
    listView->setSelectionMode(QAbstractItemView::SingleSelection); // Enable single selection

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(listView);
    setLayout(layout);

    // Set up the model and load images automatically
    setModel(imageDataModel);
    listView->setItemDelegate(new ThumbnailDelegate(this));

    // Connect the selection changed signal to the slot
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &GridView::handleSelectionChanged);

    // Connect to know new image selected
    QObject::connect(&Workspace::Instance().getActiveSession().getImageRepo(), &ImageRepository::onImageChanged, this, &GridView::onImageChanged);

    // Connect the scrollbar to load more images when necessary
    connect(listView->verticalScrollBar(), &QScrollBar::valueChanged, this, &GridView::onScroll);
}


void GridView::handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    QModelIndexList selectedIndexes = selected.indexes();
    if (!selectedIndexes.isEmpty()) {
        int selectedIndex = selectedIndexes.first().row();  // Get the first selected index
        if (selectedIndex >= 0 && selectedIndex < imageDataModel->rowCount()) {
            qDebug() << "Selected Image Row:" << selectedIndex;
            // Notify the ImageRepository about the selection change
            Workspace::Instance().getActiveSession().getImageRepo().selectImage(selectedIndex);
        }
    }
}

void GridView::setModel(ImageDataModel *model) {
    imageDataModel = model;
    listView->setModel(imageDataModel);
    initializeGrid();
}

void GridView::initializeGrid() {
    listView->setContentsMargins(20, 20, 20, 20);
    //listView->setGridSize(QSize(120 + 2 * 20, 120 + 2 * 20));  // Make sure the grid size includes padding
    listView->setViewMode(QListView::IconMode);
    listView->setSpacing(50);
    listView->setResizeMode(QListView::Adjust);
    listView->setFlow(QListView::LeftToRight);
    listView->setUniformItemSizes(true);
    listView->setGridSize(QSize(120, 120));
}

void GridView::onImageChanged(Image* newImage) {
    // if (selectedIndex >= 0 && selectedIndex < imageDataModel->rowCount()) {
    //     QModelIndex modelIndex = imageDataModel->index(selectedIndex, 0);

    //     // Set the new selection in listView
    //     listView->selectionModel()->setCurrentIndex(
    //         modelIndex, QItemSelectionModel::ClearAndSelect | QItemSelectionModel::Rows
    //         );

    //     // Scroll to the selected item to bring it into view if needed
    //     listView->scrollTo(modelIndex);
    // }
}

void GridView::onScroll(int value) {
    // Detect when the scrollbar reaches the bottom
    int maxScroll = listView->verticalScrollBar()->maximum();
    if (value == maxScroll) {
        // Load the next 20 images
        int startIndex = imageDataModel->rowCount();
        int endIndex = startIndex + 20; // Load the next 20 images (example)
        imageDataModel->loadImages(startIndex, endIndex);
    }
}
