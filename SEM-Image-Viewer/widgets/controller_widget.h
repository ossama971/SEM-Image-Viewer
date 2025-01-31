#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include "../core/data/session_data.h"
#include "../core/engines/workspace.h"
#include "history_widget.h"
#include "grayscale_widget.h"
#include "noise_reduction_widget.h"
#include "grid_view_widget.h"

class LoggerWidget;
class ImageWidget;
class SharpenWidget;
class EdgeExtractionWidget;


class Controller : public QObject
{
    Q_OBJECT

public:
    static Controller &instance()
    {
        static Controller instance;
        return instance;
    }


    void setImageWidget(ImageWidget *widget);
    void setSharpenWidget(SharpenWidget *widget);
    void setEdgeExtractionWidget(EdgeExtractionWidget *widget);
    void setNoiseReductionWidget(NoiseReductionWidget *widget);
    void setHistoryWidget(HistoryWidget *widget);
    void setGraySacleWidget(GrayScaleWidget* widget);
    void setGridView(GridView* widget);

private slots:

    void onEdgeWidgetFilterApplied();
    void onNoiseReductionFilterApplied();
    void onSharpenFilterApplied();
    void onGraySacleFilterApplied();

    void redoAction();
    void undoAction();
private:
    Controller();
    Controller(const Controller &) = delete;
    Controller &operator=(const Controller &) = delete;

    void applyFilter(std::unique_ptr<ImageFilter> filter);

    SessionData &SessionData_;
    HistoryWidget* historyWidget=nullptr;
    ImageWidget *imageWidget = nullptr;
    SharpenWidget *sharpenWidget = nullptr;
    EdgeExtractionWidget *edgeExtractionWidget = nullptr;
    NoiseReductionWidget *noiseReductionWidget = nullptr;
    GrayScaleWidget *graySacleWidget = nullptr;
    GridView *gridView;

signals:
    void imageUpdated(const cv::Mat &newImage);

};

#endif // CONTROLLER_H
