#include "topmiddlewidget.h"

TopMiddleWidget::TopMiddleWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    setStyleSheet("background-color: #00ee00;");
    setMinimumWidth(mainScreenWidth*0.3);
    setMaximumWidth(mainScreenWidth*1.0);

    ImageWidget *image = new ImageWidget();

    QVBoxLayout *topMiddleLayout = new QVBoxLayout();


    QWidget *topMiddleContent = new QWidget(parent);
    topMiddleContent->setStyleSheet("background-color: #627e7c;");

    topMiddleLayout->addWidget(topMiddleContent);
    topMiddleLayout->addWidget(image);

    this->setLayout(topMiddleLayout);
}

void TopMiddleWidget::setMaxMinHeight(int mn, int mx){
    setMinimumHeight(mn);
    setMaximumHeight(mx);
}

