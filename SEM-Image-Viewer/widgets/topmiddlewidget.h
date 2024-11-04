#ifndef TOPMIDDLEWIDGET_H
#define TOPMIDDLEWIDGET_H

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

class TopMiddleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopMiddleWidget(QWidget *parent = nullptr);

public slots:
    void setMaxMinHeight(int mn, int mx);
};

#endif // TOPMIDDLEWIDGET_H


