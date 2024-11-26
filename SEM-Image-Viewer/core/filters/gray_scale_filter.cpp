#include "gray_scale_filter.h"
#include "../engines/logger.h"
#include "../data/image.h"

cv::Mat GrayScaleFilter::applyFilter(const Image &inputImage) const
{

    const cv::Mat &image = inputImage.readImageMat();
    cv::Mat outputImage = image;

    const std::vector< std::unique_ptr<ImageState> > &states= inputImage.getStates();
    for(auto it=states.begin();it!=states.end();it++){
        if((*it)->State==ImageStateSource::GrayScaleFilter){
            //TODO: make logger msg(filter already applied)
            Logger::instance()->logMessage(
                Logger::MessageTypes::warning, Logger::MessageID::filter_applied,
                Logger::MessageOption::with_path,
                {"Gray Scale"},
                "https://docs.opencv.org/3.4/de/d25/imgproc_color_conversions.html");
            return outputImage;
        }
    }
    

    if (image.channels() != 1)
    {
        cv::cvtColor(image, outputImage, cv::COLOR_BGR2GRAY);
    }

    Logger::instance()->logMessage(
        Logger::MessageTypes::info, Logger::MessageID::images_loading_started,
        Logger::MessageOption::with_path,
        {"Gray Scale"},
        "https://docs.opencv.org/3.4/de/d25/imgproc_color_conversions.html");
    return outputImage;
}

ImageStateSource GrayScaleFilter::getImageSource() const
{
    return ImageStateSource::GrayScaleFilter;
}
