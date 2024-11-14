#ifndef BOTTOMMIDDLEWIDGET_H
#define BOTTOMMIDDLEWIDGET_H

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



class BottomMiddleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BottomMiddleWidget(QWidget *parent = nullptr);

    void setViewController(WidgetViewController* widgetViewController);

public slots:
    void setMaxMinHeight(int mn, int mx);

private:
    WidgetViewController* viewController;
};

#endif // BOTTOMMIDDLEWIDGET_H


