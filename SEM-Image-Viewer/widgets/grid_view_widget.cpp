#include "grid_view_widget.h"
#include "../core/engines/workspace.h"
#include "thumbnail_delegate.h"

#include <QVBoxLayout>
#include <QHeaderView>
#include <QMenu>
#include <QCheckBox>
#include <QStyle>
#include <QDebug>
GridView::GridView(QWidget *parent) : QWidget(parent), imageDataModel(new ImageDataModel(this)) {
    listView = new QListView(this);
    listView->setSelectionMode(QAbstractItemView::MultiSelection);

    // Initialize main layout as a grid layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    QCheckBox *selectToggleCheckbox = new QCheckBox("Select All", this);

    mainLayout->addWidget(selectToggleCheckbox);
    mainLayout->addWidget(listView);

    // Set the layout for this widget
    setLayout(mainLayout);
    connect(selectToggleCheckbox, &QCheckBox::clicked, this, [=](bool checked) {
        toggleSelectAll(selectToggleCheckbox, checked);
    });

    // To show menu bar
    listView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(listView, &QListView::customContextMenuRequested, this, &GridView::showContextMenu);

    // Set up the model and load images automatically
    setModel(imageDataModel);
    //listView->setItemDelegate(new ThumbnailDelegate(this));

    // Connect the selection changed signal to the slot
    connect(listView->selectionModel(), &QItemSelectionModel::selectionChanged, this, &GridView::handleSelectionChanged);

    // Connect to know new image selected
    QObject::connect(&Workspace::Instance()->getActiveSession().getImageRepo(), &ImageRepository::onImageChanged, this, &GridView::onImageChanged);

    // Connect the scrollbar to load more images when necessary
    connect(listView->verticalScrollBar(), &QScrollBar::valueChanged, this, &GridView::onScroll);
}

std::vector<int> GridView::getSelectedImages() {
    std::vector<int> images;

    QModelIndexList selectedIndexes = listView->selectionModel()->selectedIndexes();
    if (selectedIndexes.isEmpty())
        return images;

    for (auto it = selectedIndexes.begin(); it != selectedIndexes.end(); ++it)
        images.push_back(it->row());
    return images;
}

void GridView::handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected) {
    QModelIndexList selectedIndexes = listView->selectionModel()->selectedIndexes();
    if (!selectedIndexes.isEmpty()) {
        if (selectedIndexes.size()==1){
            int selectedIndex = selectedIndexes.first().row();  // Get the selected index
            Workspace::Instance()->getActiveSession().getImageRepo().selectImage(selectedIndex);
        }

        // Store two selected images if exactly two are selected
        if (selectedIndexes.size() == 2) {
            firstImage = imageDataModel->getImageAt(selectedIndexes[0].row());
            secondImage = imageDataModel->getImageAt(selectedIndexes[1].row());
        }

        if(selectedIndexes.size() >2){
            //send batch images to image repository
        }
    }
}

void GridView::setModel(ImageDataModel *model) {
    imageDataModel = model;
    listView->setModel(imageDataModel);
    initializeGrid();
}

void GridView::initializeGrid() {
    listView->setContentsMargins(2, 2, 2, 2);
    listView->setViewMode(QListView::IconMode);
    //listView->setSpacing(15);
    listView->setResizeMode(QListView::Adjust);
    listView->setFlow(QListView::LeftToRight);
    listView->setUniformItemSizes(true);
    listView->setGridSize(QSize(120, 120));
    auto* thumbnailDelegate = new ThumbnailDelegate(this);
    thumbnailDelegate->setPadding(10); // Set padding size
    listView->setItemDelegate(thumbnailDelegate);
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
        int startIndex = imageDataModel->rowCount();
        int endIndex = startIndex + 20; // Load the next 20 images
        imageDataModel->loadImages(startIndex, endIndex);
    }
}

void GridView::showContextMenu(const QPoint &pos) {
    QModelIndexList selectedIndexes = listView->selectionModel()->selectedIndexes();
    if (selectedIndexes.size() == 2) {
        QMenu contextMenu;
        QAction *openInDiffView = new QAction("Open in Diff View", this);
        connect(openInDiffView, &QAction::triggered, this, &GridView::openInDiffView);
        contextMenu.addAction(openInDiffView);
        contextMenu.exec(listView->viewport()->mapToGlobal(pos));
    }
}

void GridView::openInDiffView() {
    emit openDiffView();
    emit openDiffViewRequested(firstImage, secondImage);
}

void GridView::toggleSelectAll(QCheckBox *checkbox, bool checked) {
    if (checked) {
        // Select all
        listView->selectAll();
        checkbox->setText("Unselect All");
    } else {
        // Deselect all
        listView->clearSelection();
        checkbox->setText("Select All");
    }
}
