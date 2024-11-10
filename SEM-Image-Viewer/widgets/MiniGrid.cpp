#include "MiniGrid.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QScrollBar>
#include "../core/engines/Workspace.h"
#include "ThumbnailDelegate.h"

#include <QDebug>

MiniGrid::MiniGrid(QWidget *parent) : QWidget(parent), imageDataModel(new ImageDataModel(this)) {
    listView = new QListView(this);
    listView->setSelectionMode(QAbstractItemView::SingleSelection); // Enable single selection
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
    int rowHeight = 110;
    listView->setFixedHeight(rowHeight);

    // Add top margin to the listView widget to create vertical padding between the thumbnails and the widget top
    listView->setContentsMargins(2, 2, 2, 2);  // Adds padding on top of the list view

    // Connect buttons to slots
    connect(leftButton, &QPushButton::clicked, this, &MiniGrid::scrollLeft);
    connect(rightButton, &QPushButton::clicked, this, &MiniGrid::scrollRight);

    // Set up the model and initialize the grid
    setModel(imageDataModel);
    listView->setItemDelegate(new ThumbnailDelegate(this));
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &MiniGrid::handleSelectionChanged);
    QObject::connect(&Workspace::Instance().getActiveSession().getImageRepo(), &ImageRepository::onImageChanged, this, &MiniGrid::onImageChanged);
}

void MiniGrid::setModel(ImageDataModel *model) {
    imageDataModel = model;
    listView->setModel(imageDataModel);
    initializeMiniGrid();
}

void MiniGrid::initializeMiniGrid() {
    listView->setViewMode(QListView::IconMode);
    listView->setSpacing(50);  // Control the spacing between thumbnails
    listView->setResizeMode(QListView::Adjust);
    listView->setFlow(QListView::LeftToRight);
    listView->setUniformItemSizes(true);
    //listView->setGridSize(QSize(150, 150));  // Adjust this to match the size of your thumbnails


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

void MiniGrid::handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
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

void MiniGrid::onImageChanged(Image* newImage) {

}
