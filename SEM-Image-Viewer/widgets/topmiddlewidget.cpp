#include "topmiddlewidget.h"
#include "controllerWidget.h"
#include "toolbar_widget.h"

TopMiddleWidget::TopMiddleWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    //setStyleSheet("background-color: #00ee00;");
    setMinimumWidth(mainScreenWidth*0.3);
    setMaximumWidth(mainScreenWidth*1.0);

    image = new ImageWidget();
    gridView = new GridView;
    ToolbarWidget *toolbar = new ToolbarWidget(this);

    QVBoxLayout *topMiddleLayout = new QVBoxLayout();

    Controller &controller = Controller::instance();

    QWidget *topMiddleContent = new QWidget(parent);
    //topMiddleContent->setStyleSheet("background-color: #627e7c;");

    // Connect signal to open DiffView
    connect(gridView, &GridView::openDiffViewRequested, this, &TopMiddleWidget::openDiffView);
    // Connect toolbar buttons to the respective slots
    connect(toolbar->button1, &QToolButton::clicked, this, &TopMiddleWidget::onButton1Clicked);
    connect(toolbar->button3, &QToolButton::clicked, this, &TopMiddleWidget::onButton3Clicked);

    topMiddleLayout->addWidget(toolbar, 0, Qt::AlignTop);
    topMiddleLayout->addWidget(topMiddleContent);
    //topMiddleLayout->addWidget(image);
    topMiddleLayout->addWidget(gridView);

    controller.setImageWidget(image);
    this->setLayout(topMiddleLayout);
}

void TopMiddleWidget::setMaxMinHeight(int mn, int mx){
    setMinimumHeight(mn);
    setMaximumHeight(mx);
}

void TopMiddleWidget::openDiffView() {
    // topMiddleLayout->removeWidget(gridView);
    // gridView->setVisible(false);
    // topMiddleLayout->addWidget(diffView);
    // diffView->setVisible(true);
}

void TopMiddleWidget::onButton1Clicked(){
    layout()->removeWidget(gridView);
    gridView->setVisible(false);

    layout()->addWidget(image);
    image->setVisible(true);
}

void TopMiddleWidget::onButton2Clicked(){
    // Handle button 2 click
}

// Add GridView and show it
void TopMiddleWidget::onButton3Clicked() {
    layout()->removeWidget(image);
    image->setVisible(false);

    layout()->addWidget(gridView);
    gridView->setVisible(true);
}
