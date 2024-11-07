#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <opencv2/opencv.hpp>
#include "../core/engines/ImageSession.h"
#include "../core/engines/Workspace.h"
#include "historywidget.h"
class LoggerWidget;
class ImageWidget;
class ContourWidget;
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

    void setLoggerWidget(LoggerWidget *widget);
    void setImageWidget(ImageWidget *widget);
    void setContourWidget(ContourWidget *widget);
    void setEdgeExtractionWidget(EdgeExtractionWidget *widget);
    void setHistoryWidget(HistoryWidget *widget);

private slots:

    void onEdgeWidgetFilterApplied();
    void onContourFilterApplied();
    void undoAction();
    void redoAction();

private:
    Controller();
    Controller(const Controller &) = delete;
    Controller &operator=(const Controller &) = delete;
    void printMat(const cv::Mat &mat);

    ImageSession &ImageSession_;

    LoggerWidget *loggerWidget = nullptr;
    HistoryWidget* historyWidget=nullptr;
    ImageWidget *imageWidget = nullptr;
    ContourWidget *contourWidget = nullptr;
    EdgeExtractionWidget *edgeExtractionWidget = nullptr;


signals:
    void imageUpdated(const cv::Mat &newImage);

};

#endif // CONTROLLER_H
