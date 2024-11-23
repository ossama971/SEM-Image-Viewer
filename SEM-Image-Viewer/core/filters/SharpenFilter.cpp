#include "SharpenFilter.h"

cv::Mat SharpenFilter::applyFilter(const Image &inputImage) const
{

    const cv::Mat &image = inputImage.getImageMat();
    cv::Mat outputImage;
    float laplacianBoostFactor = 1.0;                                                                // Controls the amount of Laplacian Sharpening
    cv::Mat kern = (cv::Mat_<double>(3, 3) << 0, -1, 0, -1, 5 * laplacianBoostFactor, -1, 0, -1, 0); // The filtering mask for Laplacian sharpening
    filter2D(image.clone(), outputImage, image.depth(), kern, cv::Point(-1, -1));                    // Applies the masking operator to the image

    return outputImage;
}

ImageStateSource SharpenFilter::getImageSource() const
{
    return ImageStateSource::SharpenFilter;
}
