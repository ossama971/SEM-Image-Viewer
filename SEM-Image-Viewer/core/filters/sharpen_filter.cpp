#include "sharpen_filter.h"

// applyFilter method implementation
cv::Mat SharpenFilter::applyFilter(const cv::Mat& inputImage) const {

    cv::Mat outputImage;
    float laplacianBoostFactor = 1.0; // Controls the amount of Laplacian Sharpening
    cv::Mat kern = (cv::Mat_<double>(3, 3) << 0, -1, 0, -1, 5*laplacianBoostFactor, -1, 0, -1, 0); // The filtering mask for Laplacian sharpening
    filter2D(inputImage.clone(), outputImage, inputImage.depth(), kern, cv::Point(-1, -1)); // Applies the masking operator to the image
    return outputImage;

}
