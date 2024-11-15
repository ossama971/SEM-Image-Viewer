#include "SessionData.h"
#include "../data/Image.h"

void SessionData::loadDirectory(const std::string path) {
    _imageRepo.load_directory(path);
}

void SessionData::loadImage(const std::string path) {
    _imageRepo.load_image(path);

    Image* selectedImage = _imageRepo.getImage();
    if (!selectedImage)
        return;

    emit loadActionList(selectedImage->getHistory());
}

void SessionData::saveImage(const std::string path, ImageFormat format) {
    Image* selectedImage = _imageRepo.getImage();
    if (!selectedImage)
        return;

    _imageRepo.save(*selectedImage, format, path);
}



void SessionData::applyFilter(std::unique_ptr<ImageFilter> filter) {

    Image* selectedImage = _imageRepo.getImage();
    if (!selectedImage)
        return;


    selectedImage->setImage(std::move(filter->applyFilter(*selectedImage)), filter->getImageSource());
    emit updateActionList(selectedImage->GetCurrentAction());
}

std::vector<int>
SessionData::pixelIntensity(const std::vector<std::pair<int, int>> &points) {
    cv::Mat gray;
    cv::cvtColor(_imageRepo.getImage()->getImageMat(), gray, cv::COLOR_BGR2GRAY);
    std::vector<int> intensities;
    for (const auto &point : points) {
        intensities.push_back(gray.at<uchar>(point.first, point.second));
    }
    return intensities;
}

cv::Mat SessionData::heatMap() {
    cv::Mat gray;
    cv::cvtColor(_imageRepo.getImage()->getImageMat(), gray, cv::COLOR_BGR2GRAY);
    cv::applyColorMap(gray, gray, cv::COLORMAP_JET);
    return gray;
}

cv::Mat SessionData::diffTwoImages(const cv::Mat &image2,
                                    const int threshold) {
    const cv::Mat& image1 = _imageRepo.getImage()->getImageMat();
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

bool SessionData::undo(){
    Image* selectedImage = _imageRepo.getImage();
    if (!selectedImage){

        return true;
    }


    if (selectedImage->undo()){
        emit popActionList();
    }
}



bool SessionData::redo(){
    Image* selectedImage = _imageRepo.getImage();
    if (!selectedImage)
        //logger
        return false;


    if(selectedImage->redo()){
        emit updateActionList(selectedImage->GetCurrentAction());
    }
    return true;
}

ImageRepository& SessionData::getImageRepo() {
    return _imageRepo;
}

Image* SessionData::getSelectedImage(void) {
    return _imageRepo.getImage();
}
