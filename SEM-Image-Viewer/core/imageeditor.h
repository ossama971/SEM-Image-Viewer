#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <opencv2/opencv.hpp>
class ImageEditor
{

public:
    ImageEditor();
    cv::Mat ApplyFilter(const cv::Mat &inputImage);
};

#endif // IMAGEEDITOR_H
