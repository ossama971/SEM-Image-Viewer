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
    Logger::instance()->log(std::make_unique<InfoMessage>(1, boost::format("EdgeDetectionFilter::applyFilter()")));
    //const cv::Mat image =inputImage.getImageMat();



    // cv::Mat detected_edges;
    // detected_edges=ToGrayscale(image);
    // detected_edges=Denoise(image);
    // cv::Canny(detected_edges,detected_edges,threshold_low,threshold_high);

    //return detected_edges;
    cv::Vec3b tint(50, 0, 0);
    cv::Mat output = inputImage.getImageMat().clone();

    // Loop through each pixel and apply the tint
    for (int y = 0; y < output.rows; ++y) {
        for (int x = 0; x < output.cols; ++x) {
            cv::Vec3b &pixel = output.at<cv::Vec3b>(y, x);

            // Apply the tint to each channel
            pixel[0] = cv::saturate_cast<uchar>(pixel[0] + tint[0]); // Blue channel
            pixel[1] = cv::saturate_cast<uchar>(pixel[1] + tint[1]); // Green channel
            pixel[2] = cv::saturate_cast<uchar>(pixel[2] + tint[2]); // Red channel
        }
    }

    return output;
}
