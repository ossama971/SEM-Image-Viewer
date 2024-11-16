#include "leftsidebarwidget.h"


LeftSidebarWidget::LeftSidebarWidget(QWidget *parent) : QWidget(parent), viewController(nullptr) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    setMinimumWidth(mainScreenWidth*0.05);
    setMaximumWidth(mainScreenWidth*0.3);


    QVBoxLayout *leftSidebarLayout = new QVBoxLayout();


    /*QWidget *leftContent = new QWidget(parent);
    leftSidebarLayout->addWidget(leftContent);*/

    _fileBrowser = new FileBrowserWidget();
    //_fileBrowser->setStyleSheet("background-color: #627e7c;");
    //_fileBrowser->setRoot();

    leftSidebarLayout->addWidget(_fileBrowser);


    // Set layout for left sidebar
    setLayout(leftSidebarLayout);
}

void LeftSidebarWidget::setViewController(WidgetViewController* widgetViewController) {
    viewController = widgetViewController;

    if (_fileBrowser)
        _fileBrowser->setViewController(widgetViewController);
}

void LeftSidebarWidget::setMaxMinWidth(int mn, int mx){
    setMinimumWidth(mn);
    setMaximumWidth(mx);
}

