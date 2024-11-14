#include "WidgetViewController.h"
#include "leftsidebarwidget.h"
#include "rightsidebarwidget.h"
#include "topmiddlewidget.h"
#include "bottommiddlewidget.h"

WidgetViewController::WidgetViewController(LeftSidebarWidget *leftSidebar, RightSidebarWidget *rightSidebar,
                    TopMiddleWidget *topMiddle, BottomMiddleWidget *bottomMiddle,
                    QWidget* parent)
    : QWidget(parent), leftSidebarWidget(leftSidebar), rightSidebarWidget(rightSidebar), topMiddleWidget(topMiddle), bottomMiddleWidget(bottomMiddle)
{}

void WidgetViewController::showLeftBar(bool visible, bool menu_action) {
    if (leftSidebarWidget)
    {
        leftSidebarWidget->setVisible(visible);
        if (!menu_action)
            emit onLeftBarViewChanged(visible);
    }
}

void WidgetViewController::showRightBar(bool visible, bool menu_action) {
    if (rightSidebarWidget)
    {
        rightSidebarWidget->setVisible(visible);
        if (!menu_action)
            emit onRightBarViewChanged(visible);
    }
}

void WidgetViewController::showTopMiddleBar(bool visible, bool menu_action) {
    if (topMiddleWidget)
    {
        topMiddleWidget->setVisible(visible);
        if (!menu_action)
            emit onTopMiddleViewChanged(visible);
    }
}

void WidgetViewController::showBottomMiddleBar(bool visible, bool menu_action) {
    if (bottomMiddleWidget)
    {
        bottomMiddleWidget->setVisible(visible);
        if (!menu_action)
            emit onBottomMiddleViewChanged(visible);
    }
}
