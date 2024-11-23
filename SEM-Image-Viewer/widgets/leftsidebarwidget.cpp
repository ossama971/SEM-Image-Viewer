#include "leftsidebarwidget.h"


LeftSidebarWidget::LeftSidebarWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    setMinimumWidth(mainScreenWidth*0.05);
    setMaximumWidth(mainScreenWidth*0.3);


    QVBoxLayout *leftSidebarLayout = new QVBoxLayout();


    /*QWidget *leftContent = new QWidget(parent);
    leftSidebarLayout->addWidget(leftContent);*/

    _fileBrowser = new FileBrowserWidget();
    connect(_fileBrowser, &FileBrowserWidget::expand, this, &LeftSidebarWidget::expand);
    connect(_fileBrowser, &FileBrowserWidget::collapse, this, &LeftSidebarWidget::collapse);

    leftSidebarLayout->addWidget(_fileBrowser);

    leftSidebarLayout->setContentsMargins(0, 0, 0, 0);
    leftSidebarLayout->setSpacing(0);

    // Set layout for left sidebar
    setLayout(leftSidebarLayout);
}

void LeftSidebarWidget::setVisible(bool visible) {
    QWidget::setVisible(visible);

    emit onVisibilityChange(visible);
}

void LeftSidebarWidget::setMaxMinWidth(int mn, int mx){
    setMinimumWidth(mn);
    setMaximumWidth(mx);
}

void LeftSidebarWidget::expand() {
    show();
}

void LeftSidebarWidget::collapse() {
    hide();
}
