#include "ImageSession.h"
#include "../data/Image.h"

void ImageSession::loadImage(const std::string path) {
    _imageRepo.load(path);
}

cv::Mat ImageSession::applyFilter(std::unique_ptr<ImageFilter> filter) {



    auto _it =_undoManager.createFilterCommand(_imageRepo.getImage(),std::move(filter),"Hello world");
    _imageRepo.getImage().setMat(_it);
    return _imageRepo.getImage().getImageMat();
}

Image& ImageSession::getImage(void) {
    return _imageRepo.getImage();
}

const ImageMetadata ImageSession::getImageInfo(void)
{
    return _imageRepo.getImage().getMetadata();
}



std::vector<int>
ImageSession::pixelIntensity(const std::vector<std::pair<int, int>> &points) {
    cv::Mat gray;
    cv::cvtColor(_imageRepo.getImage().getImageMat(), gray, cv::COLOR_BGR2GRAY);
    std::vector<int> intensities;
    for (const auto &point : points) {
        intensities.push_back(gray.at<uchar>(point.first, point.second));
    }
    return intensities;
}

cv::Mat ImageSession::heatMap() {
    cv::Mat gray;
    cv::cvtColor(_imageRepo.getImage().getImageMat(), gray, cv::COLOR_BGR2GRAY);
    cv::applyColorMap(gray, gray, cv::COLORMAP_JET);
    return gray;
}

cv::Mat ImageSession::diffTwoImages(const cv::Mat &image2,
                                    const int threshold) {
    const cv::Mat& image1 = _imageRepo.getImage().getImageMat();
    cv::Mat diff;
    cv::absdiff(image1, image2, diff);
    cv::Mat mask(diff.rows, diff.cols, CV_8UC1, cv::Scalar(0));
    for (int j = 0; j < diff.rows; ++j) {
        for (int i = 0; i < diff.cols; ++i) {
            cv::Vec3b pix = diff.at<cv::Vec3b>(j, i);
            int val = (pix[0] + pix[1] + pix[2]);
            if (val > threshold) {
                mask.at<unsigned char>(j, i) = 255;
            }
        }
    }
    cv::Mat res;
    cv::bitwise_and(image1, image1, res, mask);
    return res;
}

cv::Mat ImageSession::undo(){
    return _undoManager.undo();
}

cv::Mat ImageSession::redo(){
    return _undoManager.redo();
}
