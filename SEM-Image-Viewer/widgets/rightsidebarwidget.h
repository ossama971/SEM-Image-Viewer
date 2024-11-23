#ifndef RIGHTSIDEBARWIDGET_H
#define RIGHTSIDEBARWIDGET_H

#include "../core/data/ImageRepository.h"
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
#include "historywidget.h"
#include "heatmapwidget.h"
#include <mutex>

class RightSidebarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RightSidebarWidget(QWidget *parent = nullptr);

public:
    void setVisible(bool visible) override;

signals:
    void onVisibilityChange(bool visible);

public slots:
    void setMaxMinWidth(int mn, int mx);


private:
    ImageRepository* _imageRepo;
    std::mutex _progressBarMtx;
    ProgressBarComponent* _progressBar;
    HistoryWidget *_historyWidget;

};

#endif // RIGHTSIDEBARWIDGET_H


