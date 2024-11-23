#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include "../core/data/SessionData.h"
#include "../core/engines/Workspace.h"
#include "historywidget.h"
#include "grayscalewidget.h"
#include "noisereductionwidget.h"
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


    SessionData &SessionData_;
    HistoryWidget* historyWidget=nullptr;
    ImageWidget *imageWidget = nullptr;
    SharpenWidget *sharpenWidget = nullptr;
    EdgeExtractionWidget *edgeExtractionWidget = nullptr;
    NoiseReductionWidget *noiseReductionWidget = nullptr;
    GrayScaleWidget *graySacleWidget = nullptr;

signals:
    void imageUpdated(const cv::Mat &newImage);

};

#endif // CONTROLLER_H
