#ifndef TOPMIDDLEWIDGET_H
#define TOPMIDDLEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGuiApplication>
#include <QScreen>
#include <QToolButton>

#include "image_widget.h"
#include "GridView.h"

class TopMiddleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopMiddleWidget(QWidget *parent = nullptr);

public slots:
    void setMaxMinHeight(int mn, int mx);
    void openDiffView();

private slots:
    void onButton1Clicked();
    void onButton2Clicked();
    void onButton3Clicked();

private:
    QWidget *toolbar;
    QPushButton *button1;
    QPushButton *button2;
    QPushButton *button3;
    ImageWidget *image;
    GridView *gridView;

};

#endif // TOPMIDDLEWIDGET_H
