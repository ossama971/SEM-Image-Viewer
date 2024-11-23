#include "bottommiddlewidget.h"
#include "LoggerWidget.h"
#include "loggercontroller.h"
BottomMiddleWidget::BottomMiddleWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    //setStyleSheet("background-color: #000000;");
    setMinimumWidth(mainScreenWidth*0.3);
    setMaximumWidth(mainScreenWidth*1.0);

    QVBoxLayout *bottomMiddleLayout = new QVBoxLayout();



    QWidget *bottomMiddleContent = new QWidget(parent);
    //bottomMiddleContent->setStyleSheet("background-color: #627e7c;");
    bottomMiddleLayout->addWidget(bottomMiddleContent);
    LoggerWidget * logger = new LoggerWidget();
    bottomMiddleLayout->addWidget(logger);

    loggerController::instance().setLoggerWidget(logger);
    this->setLayout(bottomMiddleLayout);
}

void BottomMiddleWidget::setVisible(bool visible) {
    QWidget::setVisible(visible);

    emit onVisibilityChange(visible);
}

void BottomMiddleWidget::setMaxMinHeight(int mn, int mx){
    setMinimumHeight(mn);
    setMaximumHeight(mx);
}
