#include "topmiddlewidget.h"
#include "controllerWidget.h"

#include <QDebug>

TopMiddleWidget::TopMiddleWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    //setStyleSheet("background-color: #00ee00;");
    setMinimumWidth(mainScreenWidth*0.3);
    setMaximumWidth(mainScreenWidth*1.0);

    image = new ImageWidget();
    gridView = new GridView();
    diffView = new DiffViewWidget();
    topMiddleLayout = new QVBoxLayout();

    Controller &controller = Controller::instance();

    // QWidget *topMiddleContent = new QWidget(parent);
    //topMiddleContent->setStyleSheet("background-color: #627e7c;");

    // Connect signal to open DiffView
    connect(gridView, &GridView::openDiffView, this, &TopMiddleWidget::openDiffView);
    connect(gridView, &GridView::openDiffViewRequested, diffView, &DiffViewWidget::setImages);

    // topMiddleLayout->addWidget(topMiddleContent);
    // topMiddleLayout->addWidget(diffView);
    // topMiddleLayout->addWidget(image);
    topMiddleLayout->addWidget(gridView);

    controller.setImageWidget(image);
    this->setLayout(topMiddleLayout);
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
}
