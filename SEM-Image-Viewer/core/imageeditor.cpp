#include "imageeditor.h"

ImageEditor::ImageEditor() {}

cv::Mat ImageEditor::ApplyFilter(const cv::Mat &inputImage){
    //cv::Mat _outimage;
   // cv::GaussianBlur(inputImage, _outimage, cv::Size(5, 5), 0);

    cv::Mat output = inputImage.clone(); // Create a copy of the input image
    cv::Vec3b tint(50, 0, 0);
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
