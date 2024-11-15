#include "GrayScaleFilter.h"



cv::Mat GrayScaleFilter::applyFilter(const Image &inputImage) const
{
    Logger::instance()->log(std::make_unique<InfoMessage>(1, boost::format("Converting image to grayscale")));
    const cv::Mat &image = inputImage.getImageMat();
    cv::Mat outputImage = image;

    if(image.channels() !=1){
       cv::cvtColor(image, outputImage, cv::COLOR_BGR2GRAY);
    }

    return outputImage;
}


ImageStateSource GrayScaleFilter::getImageSource() const {
    return ImageStateSource::GrayScaleFilter;
}
