#ifndef RIGHT_SIDE_BAR_WIDGET_H
#define RIGHT_SIDE_BAR_WIDGET_H

#include "../core/data/image_repository.h"
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
#include "progress_bar.h"
#include "history_widget.h"
#include "heat_map_widget.h"
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

#endif // RIGHT_SIDE_BAR_WIDGET_H


