#ifndef HEAT_MAP_WIDGET_H
#define HEAT_MAP_WIDGET_H
#include "../core/utils.h"
#include <QWidget>
#include <QToolButton>

class HeatMapWidget : public QWidget
{
    Q_OBJECT
public:
    explicit HeatMapWidget(QWidget *parent = nullptr);
private:
    QToolButton *heatmapButton;
signals:
    void applyHeatMap(cv::Mat heatmap,bool checked);
public slots:
    void handleApplyHeatMap(bool checked);
    void resetToggle();
};

#endif // HEAT_MAP_WIDGET_H
