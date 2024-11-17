#include "GrayScaleFilter.h"



cv::Mat GrayScaleFilter::applyFilter(const Image &inputImage) const
{

    const cv::Mat &image = inputImage.getImageMat();
    cv::Mat outputImage = image;

    if(image.channels() !=1){
       cv::cvtColor(image, outputImage, cv::COLOR_BGR2GRAY);
    }
    Logger::instance()->log(std::make_unique<InfoMessage>(LOG_INFO,boost::format(LogMessageMapper::filterApplied("Gray Scale Filter").toStdString())));
    return outputImage;
}


ImageStateSource GrayScaleFilter::getImageSource() const {
    return ImageStateSource::GrayScaleFilter;
}
