#ifndef RIGHTSIDEBARWIDGET_H
#define RIGHTSIDEBARWIDGET_H

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
#include "progressbar.h"

class RightSidebarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightSidebarWidget(QWidget *parent = nullptr);
public slots:
    void setMaxMinWidth(int mn, int mx);
    void initializeProgress(int maxIterations);
    void updateProgress();
    void hideProgressBar();
private:
    ProgressBarComponent* _progressBar;

};

#endif // RIGHTSIDEBARWIDGET_H


