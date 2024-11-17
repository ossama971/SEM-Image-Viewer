#include "topmiddlewidget.h"
#include "controllerWidget.h"

TopMiddleWidget::TopMiddleWidget(QWidget *parent)
  : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    //setStyleSheet("background-color: #00ee00;");
    setMinimumWidth(mainScreenWidth*0.3);
    setMaximumWidth(mainScreenWidth*1.0);

    image = new ImageWidget();
    gridView = new GridView();
    diffView = new DiffViewWidget();
    topMiddleLayout = new QVBoxLayout();
    toolbar = new ToolbarWidget();

    QWidget *topMiddleContent = new QWidget(parent);
    connect(gridView, &GridView::openDiffView, this, &TopMiddleWidget::openDiffView);
    connect(gridView, &GridView::openDiffViewRequested, diffView, &DiffViewWidget::setImages);
    
    connect(toolbar->imageViewButton, &QToolButton::clicked, this, &TopMiddleWidget::onimageViewButtonClicked);
    connect(toolbar->diffViewButton, &QToolButton::clicked, this, &TopMiddleWidget::ondiffViewButtonClicked);
    connect(toolbar->gridViewButton, &QToolButton::clicked, this, &TopMiddleWidget::ongridViewButtonClicked);

    topMiddleLayout->addWidget(toolbar, 0, Qt::AlignTop);
    topMiddleLayout->addWidget(topMiddleContent);
    topMiddleLayout->addWidget(image);

    Controller::instance().setImageWidget(image);
    this->setLayout(topMiddleLayout);

    // Connect to tell toolbar that diff view is opened when to images is selected and right clicked to open in diff view
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
