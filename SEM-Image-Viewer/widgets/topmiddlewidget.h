#ifndef TOPMIDDLEWIDGET_H
#define TOPMIDDLEWIDGET_H

#include "WidgetViewController.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGuiApplication>
#include <QScreen>

#include "image_widget.h"
#include "GridView.h"
#include "DiffViewWidget.h"

class TopMiddleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopMiddleWidget(QWidget *parent = nullptr);

    void setViewController(WidgetViewController* widgetViewController);

public slots:
    void setMaxMinHeight(int mn, int mx);
    void openDiffView();

private:
    QWidget *toolbar;
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


