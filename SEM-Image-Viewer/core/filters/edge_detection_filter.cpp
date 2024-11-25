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
cv::Mat EdgeDetectionFilter::applyFilter(const Image &inputImage) const
{

    const cv::Mat image = inputImage.getImageMat();

    cv::Mat detected_edges = image;
    if (image.channels() != 1)
    {

        detected_edges = toGrayscale(image);
    }
    detected_edges = denoise(detected_edges);
    cv::Canny(detected_edges, detected_edges, threshold_low, threshold_high);

    Logger::instance()->logMessage(
        Logger::MessageTypes::info, Logger::MessageID::images_loading_started,
        Logger::MessageOption::with_path,
        {"Edge Detection"},
        "https://docs.opencv.org/4.x/da/d22/tutorial_py_canny.html");

    return detected_edges;
}

ImageStateSource EdgeDetectionFilter::getImageSource() const
{
    return ImageStateSource::EdgeDetectionFilter;
}
