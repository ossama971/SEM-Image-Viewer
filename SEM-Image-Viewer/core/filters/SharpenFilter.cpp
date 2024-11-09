#include "SharpenFilter.h"

cv::Mat SharpenFilter::applyFilter(const Image& inputImage) const {

    const cv::Mat& image = inputImage.getImageMat();
    cv::Mat outputImage;
    float laplacianBoostFactor = 1.0; // Controls the amount of Laplacian Sharpening
    cv::Mat kern = (cv::Mat_<double>(3, 3) << 0, -1, 0, -1, 5*laplacianBoostFactor, -1, 0, -1, 0); // The filtering mask for Laplacian sharpening
    filter2D(image.clone(), outputImage, image.depth(), kern, cv::Point(-1, -1)); // Applies the masking operator to the image
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

    //return output;

}
