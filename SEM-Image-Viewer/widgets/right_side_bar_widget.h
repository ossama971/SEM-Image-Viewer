#ifndef RIGHT_SIDE_BAR_WIDGET_H
#define RIGHT_SIDE_BAR_WIDGET_H

#include "../core/data/image_repository.h"
#include "progress_bar.h"
#include "history_widget.h"
#include "heat_map_widget.h"
#include <QWidget>
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
    ImageRepository* _imageRepo = nullptr;
    std::mutex _progressBarMtx;
    ProgressBarComponent* _progressBar = nullptr;
    HistoryWidget *_historyWidget = nullptr;

};

#endif // RIGHT_SIDE_BAR_WIDGET_H


