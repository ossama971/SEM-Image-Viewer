#ifndef LEFTSIDEBARWIDGET_H
#define LEFTSIDEBARWIDGET_H

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


class LeftSidebarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftSidebarWidget(QWidget *parent = nullptr);

public slots:
    void setMaxMinWidth(int mn, int mx);
};

#endif // LEFTSIDEBARWIDGET_H


