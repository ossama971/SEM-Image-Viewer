#ifndef BATCHFILTER_H
#define BATCHFILTER_H

#include "ImageFilter.h"
#include <QWidget>
#include <memory>
#include <vector>
#include <QDateTime>
#include "../engines/Logger.h"


class BatchFilter : public QObject
{
    Q_OBJECT

public:
    BatchFilter(bool isolateMainThread = true, int threadCount = 0);

    void apply(std::unique_ptr<ImageFilter> filter, std::vector<Image*> input);

private:
    void execute(std::unique_ptr<ImageFilter> filter, std::vector<Image*> input);

signals:
    void onImageProcessed(Image* image);
    void onFinish(std::vector<Image*> input, std::vector<cv::Mat> output, ImageStateSource stateSource);

private:
    bool _isolateMainThread;
    int _threadCount;
    QDateTime startTime;
};

#endif // BATCHFILTER_H
