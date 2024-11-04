#include "leftsidebarwidget.h"


LeftSidebarWidget::LeftSidebarWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    setStyleSheet("background-color: #000000;");
    setMinimumWidth(mainScreenWidth*0.05);
    setMaximumWidth(mainScreenWidth*0.3);


    QVBoxLayout *leftSidebarLayout = new QVBoxLayout();


    QWidget *leftContent = new QWidget(parent);
    leftContent->setStyleSheet("background-color: #627e7c;");
    leftSidebarLayout->addWidget(leftContent);


    // Set layout for left sidebar
    this->setLayout(leftSidebarLayout);
}

void LeftSidebarWidget::setMaxMinWidth(int mn, int mx){
    setMinimumWidth(mn);
    setMaximumWidth(mx);
}

