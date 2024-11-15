#ifndef WIDGETVIEWCONTROLLER_H
#define WIDGETVIEWCONTROLLER_H

#include <QWidget>

class WidgetViewController : public QWidget
{
    Q_OBJECT

public:
    WidgetViewController(class LeftSidebarWidget *leftSidebar, class RightSidebarWidget *rightSidebar,
                         class TopMiddleWidget *topMiddle, class BottomMiddleWidget *bottomMiddle,
                         QWidget* parent = nullptr);

    void showLeftBar(bool visible, bool menu_action = false);
    void showRightBar(bool visible, bool menu_action = false);
    void showTopMiddleBar(bool visible, bool menu_action = false);
    void showBottomMiddleBar(bool visible, bool menu_action = false);

signals:
    void onLeftBarViewChanged(bool state);
    void onRightBarViewChanged(bool state);
    void onTopMiddleViewChanged(bool state);
    void onBottomMiddleViewChanged(bool state);

private:
    LeftSidebarWidget *leftSidebarWidget;
    RightSidebarWidget *rightSidebarWidget;
    TopMiddleWidget *topMiddleWidget;
    BottomMiddleWidget *bottomMiddleWidget;
};

#endif // WIDGETVIEWCONTROLLER_H
