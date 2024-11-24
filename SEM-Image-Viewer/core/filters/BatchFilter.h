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
    BatchFilter() = default;
    ~BatchFilter() = default;

    void apply(std::unique_ptr<ImageFilter> filter, std::vector<Image*> input);

signals:
    void onImageProcessed(Image* image);
    void onFinish(std::vector<Image*> input, std::vector<cv::Mat> output, ImageStateSource stateSource);

private:
    QDateTime startTime;
};

#endif // BATCHFILTER_H
