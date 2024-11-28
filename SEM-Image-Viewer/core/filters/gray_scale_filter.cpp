#include "gray_scale_filter.h"
#include "../engines/logger.h"
#include "../data/image.h"

bool GrayScaleFilter::applyFilter(const Image &inputImage, cv::Mat &outputImage, bool log) const
{
    const std::vector< std::unique_ptr<ImageState> > &states= inputImage.getStates();
    for(auto it=states.begin();it!=states.end();it++){
        if((*it)->State==ImageStateSource::GrayScaleFilter)
        {
            if (log)
            {
                Logger::instance()->logMessage(
                    Logger::MessageTypes::warning, Logger::MessageID::already_applied,
                    Logger::MessageOption::with_path,
                    {"Gray Scale"},
                    "https://docs.opencv.org/3.4/de/d25/imgproc_color_conversions.html");
            }
            return false;
        }
    }
    
    const cv::Mat &image = inputImage.readImageMat();
    outputImage = image;

    if (image.channels() != 1)
    {
        cv::cvtColor(image, outputImage, cv::COLOR_BGR2GRAY);
    }

    if (log)
    {
        Logger::instance()->logMessage(
            Logger::MessageTypes::info, Logger::MessageID::filter_applied,
            Logger::MessageOption::with_path,
            {"Gray Scale"},
            "https://docs.opencv.org/3.4/de/d25/imgproc_color_conversions.html");
    }
    return true;
}

ImageStateSource GrayScaleFilter::getImageSource() const
{
    return ImageStateSource::GrayScaleFilter;
}
