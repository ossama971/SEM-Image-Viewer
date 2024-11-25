#include "gray_scale_filter.h"
#include "../engines/logger.h"

cv::Mat GrayScaleFilter::applyFilter(const Image &inputImage) const
{

    const cv::Mat &image = inputImage.getImageMat();
    cv::Mat outputImage = image;

    if(image.channels() !=1){
       cv::cvtColor(image, outputImage, cv::COLOR_BGR2GRAY);
    }

    //Logger::instance()->logMessage("F201",{"Gray Scale"});
    return outputImage;
}


ImageStateSource GrayScaleFilter::getImageSource() const {
    return ImageStateSource::GrayScaleFilter;
}
