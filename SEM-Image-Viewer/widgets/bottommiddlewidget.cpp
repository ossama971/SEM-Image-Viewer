#include "bottommiddlewidget.h"
#include "LoggerWidget.h"
#include "controllerWidget.h"
BottomMiddleWidget::BottomMiddleWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    //setStyleSheet("background-color: #000000;");
    setMinimumWidth(mainScreenWidth*0.3);
    setMaximumWidth(mainScreenWidth*1.0);

    QVBoxLayout *bottomMiddleLayout = new QVBoxLayout();

    Controller &controller = Controller::instance();

    LoggerWidget * logger = new LoggerWidget();
    connect(logger, &LoggerWidget::layoutSwitched, this, &BottomMiddleWidget::adjustSizeBasedOnLayout);
    bottomMiddleLayout->addWidget(logger, 1);
    bottomMiddleLayout->setContentsMargins(0, 0, 0, 0);
    bottomMiddleLayout->setSpacing(0);

    controller.setLoggerWidget(logger);
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

void BottomMiddleWidget::adjustSizeBasedOnLayout(bool isExpanded)
{
    if (isExpanded) {
        setMaxMinHeight(80, 400);
    } else {
        setMaxMinHeight(40, 40);
    }
}

