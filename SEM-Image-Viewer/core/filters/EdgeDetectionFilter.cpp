#include "EdgeDetectionFilter.h"

EdgeDetectionFilter::EdgeDetectionFilter() {}
void EdgeDetectionFilter::setTHresholdHigh(int threshold)
{
    this->threshold_high=threshold;
}
void EdgeDetectionFilter::setThresholdLow(int threshold)
{
    this->threshold_low=threshold;
}
int EdgeDetectionFilter::get_threshold_low()
{
    return this->threshold_low;
}
int EdgeDetectionFilter::get_threshold_high()
{
    return this->threshold_high;
}
cv::Mat EdgeDetectionFilter::Denoise(const cv::Mat &image) const
{
    cv::Mat denoised_image;
    cv::GaussianBlur(image,denoised_image,{5,5},0);
    return denoised_image;
}
cv::Mat EdgeDetectionFilter::ToGrayscale(const cv::Mat &image) const
{
    cv::Mat gray_image;
    cv::cvtColor(image,gray_image, cv::COLOR_BGR2GRAY);
    return gray_image;
}
cv::Mat EdgeDetectionFilter::applyFilter(const Image &inputImage) const
{
    const cv::Mat image =inputImage.getImageMat();
    cv::Mat detected_edges;
    detected_edges=ToGrayscale(image);
    detected_edges=Denoise(image);
    cv::Canny(detected_edges,detected_edges,threshold_low,threshold_high);
    return detected_edges;
}
