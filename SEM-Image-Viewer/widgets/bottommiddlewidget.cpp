#include "bottommiddlewidget.h"
#include "LoggerWidget.h"
#include "controllerWidget.h"
BottomMiddleWidget::BottomMiddleWidget(QWidget *parent) : QWidget(parent), viewController(nullptr) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    //setStyleSheet("background-color: #000000;");
    setMinimumWidth(mainScreenWidth*0.3);
    setMaximumWidth(mainScreenWidth*1.0);

    QVBoxLayout *bottomMiddleLayout = new QVBoxLayout();

    Controller &controller = Controller::instance();

    QWidget *bottomMiddleContent = new QWidget(parent);
    //bottomMiddleContent->setStyleSheet("background-color: #627e7c;");
    bottomMiddleLayout->addWidget(bottomMiddleContent);
    LoggerWidget * logger = new LoggerWidget();
    bottomMiddleLayout->addWidget(logger);

    controller.setLoggerWidget(logger);
    this->setLayout(bottomMiddleLayout);
}

void BottomMiddleWidget::setViewController(WidgetViewController* widgetViewController) {
    viewController = widgetViewController;
}

void BottomMiddleWidget::setMaxMinHeight(int mn, int mx){
    setMinimumHeight(mn);
    setMaximumHeight(mx);
}
