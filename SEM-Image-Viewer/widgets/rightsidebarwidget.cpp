#include "rightsidebarwidget.h"

RightSidebarWidget::RightSidebarWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    setStyleSheet("background-color: #000000;");
    setMinimumWidth(mainScreenWidth*0.05);
    setMaximumWidth(mainScreenWidth*0.3);

    QVBoxLayout *rightSidebarLayout = new QVBoxLayout();


    QWidget *rightContent = new QWidget(parent);
    rightContent->setStyleSheet("background-color: #627e7c;");
    rightSidebarLayout->addWidget(rightContent);

    this->setLayout(rightSidebarLayout);
}



void RightSidebarWidget::setMaxMinWidth(int mn, int mx){
    setMinimumWidth(mn);
    setMaximumWidth(mx);
}
