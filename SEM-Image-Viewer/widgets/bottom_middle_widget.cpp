#include "bottom_middle_widget.h"
#include "logger_widget.h"
#include "logger_controller.h"
#include <QVBoxLayout>
#include <QVBoxLayout>
#include <QGuiApplication>
#include <QScreen>

BottomMiddleWidget::BottomMiddleWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    setMinimumWidth(mainScreenWidth*0.3);
    setMaximumWidth(mainScreenWidth*1.0);

    QVBoxLayout *bottomMiddleLayout = new QVBoxLayout();

    LoggerWidget * logger = new LoggerWidget();
    connect(logger, &LoggerWidget::layoutSwitched, this, &BottomMiddleWidget::adjustSizeBasedOnLayout);
    bottomMiddleLayout->addWidget(logger, 1);
    bottomMiddleLayout->setContentsMargins(0, 0, 0, 0);
    bottomMiddleLayout->setSpacing(0);

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

void BottomMiddleWidget::adjustSizeBasedOnLayout(bool isExpanded)
{
    if (isExpanded) {
        setMaxMinHeight(80, 400);
    } else {
        setMaxMinHeight(40, 40);
    }
}

