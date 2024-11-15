#include "GrayScaleFilter.h"



cv::Mat GrayScaleFilter::applyFilter(const Image &inputImage) const
{
     Logger::instance()->log(std::make_unique<InfoMessage>(1, boost::format("Converting image to grayscale")));
    const cv::Mat &image = inputImage.getImageMat();
    cv::Mat outputImage;

    // Convert the input image to grayscale
    cv::cvtColor(image, outputImage, cv::COLOR_BGR2GRAY);

    return outputImage;


    // cv::Vec3b warmTint(20, 10, -10); // Add to red and green, subtract from blue
    // cv::Mat output = inputImage.getImageMat().clone();


    // for (int y = 0; y < output.rows; ++y) {
    //     for (int x = 0; x < output.cols; ++x) {
    //         cv::Vec3b &pixel = output.at<cv::Vec3b>(y, x);

    //         // Apply the warm tint to each channel
    //         pixel[0] = cv::saturate_cast<uchar>(pixel[0] + warmTint[0]); // Blue channel
    //         pixel[1] = cv::saturate_cast<uchar>(pixel[1] + warmTint[1]); // Green channel
    //         pixel[2] = cv::saturate_cast<uchar>(pixel[2] + warmTint[2]); // Red channel
    //     }
    // }

    // return output;
}


ImageStateSource GrayScaleFilter::getImageSource() const {
    return ImageStateSource::GrayScaleFilter;
}
