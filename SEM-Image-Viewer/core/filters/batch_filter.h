#ifndef BATCH_FILTER_H
#define BATCH_FILTER_H

#include "image_filter.h"
#include <QWidget>
#include <memory>
#include <vector>
#include <QDateTime>
#include "../engines/logger.h"


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

#endif // BATCH_FILTER_H
