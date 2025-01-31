#include "top_middle_widget.h"
#include "controller_widget.h"

#include <QGuiApplication>
#include <QScreen>

TopMiddleWidget::TopMiddleWidget(QWidget *parent, ToolbarWidget *toolbarWidget, ImageDataModel *gridDataModel)
  : QWidget(parent), toolbar(toolbarWidget) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    setMinimumWidth(mainScreenWidth*0.3);
    setMaximumWidth(mainScreenWidth*1.0);

    image = new ImageWidget();
    gridView = new GridView(nullptr, gridDataModel);
    diffView = new DiffViewWidget();
    topMiddleLayout = new QVBoxLayout();
    topMiddleLayout->addWidget(image, 1);
    topMiddleLayout->setContentsMargins(0, 0, 0, 0);
    topMiddleLayout->setSpacing(0);

    connect(gridView, &GridView::openDiffView, this, &TopMiddleWidget::openDiffView);
    connect(gridView, &GridView::openDiffViewRequested, diffView, &DiffViewWidget::setImages);
    // connect(gridView, &GridView::resetDiffView, diffView, &DiffViewWidget::resetDiff);
    
    Controller &controller = Controller::instance();
    controller.setImageWidget(image);
    controller.setGridView(gridView);

    this->setLayout(topMiddleLayout);

    // Connect to tell toolbar that diff view is opened when to images is selected and right cl icked to open in diff view
    connect(this, &TopMiddleWidget::selectDiffView, toolbar, &ToolbarWidget::onSelectDiffView);
}

void TopMiddleWidget::setVisible(bool visible) {
    QWidget::setVisible(visible);

    emit onVisibilityChange(visible);
}

void TopMiddleWidget::setMaxMinHeight(int mn, int mx){
    setMinimumHeight(mn);
    setMaximumHeight(mx);
}

void TopMiddleWidget::openDiffView() {
    topMiddleLayout->removeWidget(gridView);
    gridView->setVisible(false);
    topMiddleLayout->addWidget(diffView);
    diffView->setVisible(true);
    emit selectDiffView();
}

void TopMiddleWidget::onimageViewButtonClicked(){
    layout()->removeWidget(gridView);
    gridView->setVisible(false);
    topMiddleLayout->removeWidget(diffView);
    diffView->setVisible(false);

    layout()->addWidget(image);
    image->setVisible(true);
}

void TopMiddleWidget::ondiffViewButtonClicked(){
    topMiddleLayout->removeWidget(gridView);
    gridView->setVisible(false);
    layout()->removeWidget(image);
    image->setVisible(false);

    topMiddleLayout->addWidget(diffView);
    diffView->setVisible(true);
}

void TopMiddleWidget::ongridViewButtonClicked() {
    layout()->removeWidget(image);
    image->setVisible(false);
    topMiddleLayout->removeWidget(diffView);
    diffView->setVisible(false);

    layout()->addWidget(gridView);
    gridView->setVisible(true);
}
