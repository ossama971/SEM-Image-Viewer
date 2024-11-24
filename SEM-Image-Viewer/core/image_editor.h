#ifndef IMAGE_EDITOR_H
#define IMAGE_EDITOR_H

#include <opencv2/opencv.hpp>
class ImageEditor
{

public:
    ImageEditor();
    //input mat , pointer
    cv::Mat ApplyFilter(const cv::Mat &inputImage);
};

#endif // IMAGE_EDITOR_H
