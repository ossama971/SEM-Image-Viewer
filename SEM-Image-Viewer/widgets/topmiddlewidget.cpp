#include "topmiddlewidget.h"
#include "controllerWidget.h"

TopMiddleWidget::TopMiddleWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    //setStyleSheet("background-color: #00ee00;");
    setMinimumWidth(mainScreenWidth*0.3);
    setMaximumWidth(mainScreenWidth*1.0);

    ImageWidget *image = new ImageWidget();
    GridView *gridView = new GridView();
    DiffViewWidget *diffView = new DiffViewWidget();

    QVBoxLayout *topMiddleLayout = new QVBoxLayout();

    Controller &controller = Controller::instance();

    QWidget *topMiddleContent = new QWidget(parent);
    //topMiddleContent->setStyleSheet("background-color: #627e7c;");

    topMiddleLayout->addWidget(topMiddleContent);
    topMiddleLayout->addWidget(diffView);
    // topMiddleLayout->addWidget(image);
    //topMiddleLayout->addWidget(gridView);
    controller.setImageWidget(image);
    this->setLayout(topMiddleLayout);
}

void TopMiddleWidget::setMaxMinHeight(int mn, int mx){
    setMinimumHeight(mn);
    setMaximumHeight(mx);
}

