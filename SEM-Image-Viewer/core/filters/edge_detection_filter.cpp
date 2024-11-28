#include "edge_detection_filter.h"
#include "../engines/logger.h"

EdgeDetectionFilter::EdgeDetectionFilter() {}
void EdgeDetectionFilter::setTHresholdHigh(int threshold)
{
    this->threshold_high = threshold;
}
void EdgeDetectionFilter::setThresholdLow(int threshold)
{
    this->threshold_low = threshold;
}
int EdgeDetectionFilter::get_threshold_low()
{
    return this->threshold_low;
}
int EdgeDetectionFilter::get_threshold_high()
{
    return this->threshold_high;
}
cv::Mat EdgeDetectionFilter::denoise(const cv::Mat &image) const
{
    cv::Mat denoised_image;
    cv::GaussianBlur(image, denoised_image, {5, 5}, 0);
    return denoised_image;
}
cv::Mat EdgeDetectionFilter::toGrayscale(const cv::Mat &image) const
{
    cv::Mat gray_image;
    cv::cvtColor(image, gray_image, cv::COLOR_BGR2GRAY);
    return gray_image;
}
bool EdgeDetectionFilter::applyFilter(const Image &inputImage, cv::Mat &outputImage, bool log) const
{

    const cv::Mat& image = inputImage.readImageMat();

    const std::vector< std::unique_ptr<ImageState> > &states= inputImage.getStates();
    for(auto it=states.begin();it!=states.end();it++){
        if((*it)->State==ImageStateSource::EdgeDetectionFilter)
        {
            if (log)
            {
                Logger::instance()->logMessage(
                    Logger::MessageTypes::warning, Logger::MessageID::already_applied,
                    Logger::MessageOption::with_path,
                    {"Edge Detection"},
                    "https://docs.opencv.org/3.4/de/d25/imgproc_color_conversions.html");
            }
            return false;
        }
    }

    outputImage = image;
    if (image.channels() != 1)
    {

        outputImage = toGrayscale(image);
    }
    outputImage = denoise(outputImage);
    cv::Canny(outputImage, outputImage, threshold_low, threshold_high);

    if (log)
    {
        Logger::instance()->logMessage(
            Logger::MessageTypes::info, Logger::MessageID::filter_applied,
            Logger::MessageOption::with_path,
            {"Edge Detection"},
            "https://docs.opencv.org/4.x/da/d22/tutorial_py_canny.html");
    }
    return true;
}

ImageStateSource EdgeDetectionFilter::getImageSource() const
{
    return ImageStateSource::EdgeDetectionFilter;
}
