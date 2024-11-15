#ifndef TOPMIDDLEWIDGET_H
#define TOPMIDDLEWIDGET_H

#include "WidgetViewController.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGuiApplication>
#include <QScreen>
#include <QToolButton>

#include "image_widget.h"
#include "GridView.h"
#include "DiffViewWidget.h"
#include "toolbar_widget.h"

class TopMiddleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopMiddleWidget(QWidget *parent = nullptr);

    void setViewController(WidgetViewController* widgetViewController);

public slots:
    void setMaxMinHeight(int mn, int mx);
    void openDiffView();

private slots:
    void onButton1Clicked();
    void onButton2Clicked();
    void onButton3Clicked();

private:
    ToolbarWidget *toolbar;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    ImageWidget *image;
    GridView *gridView;
    DiffViewWidget *diffView;
    QVBoxLayout *topMiddleLayout;

private:
    WidgetViewController* viewController;
};

#endif // TOPMIDDLEWIDGET_H
