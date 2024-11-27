#include "mini_grid_widget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollBar>
#include "../core/engines/workspace.h"
#include "../models/image_data_model.h"
#include "thumbnail_delegate.h"

MiniGrid::MiniGrid(QWidget *parent, ImageDataModel *dataModel) : QWidget(parent), imageDataModel(dataModel) {
    listView = new QListView(this);
    listView->setSelectionMode(QAbstractItemView::SingleSelection); // Enable single selection
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    // Initialize left and right buttons
    leftButton = new QPushButton("<",this);
    rightButton = new QPushButton(">",this);

    // Apply styles to buttons
    leftButton->setObjectName("leftbutton");

    rightButton->setObjectName("leftbutton");

    // Add buttons to buttonLayout
    buttonLayout->addWidget(leftButton);
    buttonLayout->addWidget(listView);
    buttonLayout->addWidget(rightButton);

    // Add buttonLayout to mainLayout
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);


    // Connect buttons to slots
    connect(leftButton, &QPushButton::clicked, this, &MiniGrid::scrollLeft);
    connect(rightButton, &QPushButton::clicked, this, &MiniGrid::scrollRight);

    // Set up the model and initialize the grid
    setModel(imageDataModel);
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MiniGrid::handleSelectionChanged);
    QObject::connect(&Workspace::Instance()->getActiveSession().getImageRepo(), &ImageRepository::onImageChanged, this, &MiniGrid::onImageChanged);

    connect(imageDataModel, &ImageDataModel::preImageStateUpdate, this, &MiniGrid::preImageStateUpdate);
    connect(imageDataModel, &ImageDataModel::postImageStateUpdate, this, &MiniGrid::postImageStateUpdate);
}

void MiniGrid::setModel(ImageDataModel *model) {
    imageDataModel = model;
    listView->setModel(imageDataModel);
    initializeMiniGrid();
}

void MiniGrid::initializeMiniGrid() {
    listView->setViewMode(QListView::IconMode);
    listView->setResizeMode(QListView::Adjust);
    listView->setFlow(QListView::LeftToRight);
    listView->setWrapping(false);
    listView->setUniformItemSizes(true);
    listView->setGridSize(QSize(100, 100));
    listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    listView->itemAlignment();
    auto* thumbnailDelegate = new ThumbnailDelegate(this);
    thumbnailDelegate->setPadding(6);
    listView->setItemDelegate(thumbnailDelegate);
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

void MiniGrid::handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    QModelIndexList selectedIndexes = selected.indexes();
    if (!selectedIndexes.isEmpty()) {
        int selectedIndex = selectedIndexes.first().row();  // Get the first selected index
        if (selectedIndex >= 0 && selectedIndex < imageDataModel->rowCount()) {
            // Notify the ImageRepository about the selection change
            Workspace::Instance()->getActiveSession().getImageRepo().selectImage(selectedIndex);
        }
    }
}

void MiniGrid::onImageChanged(Image* newImage) {

}

void MiniGrid::preImageStateUpdate() {
    QModelIndexList selectedIndexes = listView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() != 1)
        _selectedIndex = QModelIndex();
    else
        _selectedIndex = selectedIndexes[0];
}

void MiniGrid::postImageStateUpdate() {
    if (_selectedIndex.row() != -1)
    {
        QItemSelection selection(_selectedIndex, _selectedIndex);
        listView->selectionModel()->select(selection, QItemSelectionModel::Select);
    }
}
