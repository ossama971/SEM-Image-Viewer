#include "session_data.h"
#include "../data/image.h"
#include "../filters/batch_filter.h"
#include "../filters/gray_scale_filter.h"
#include "../filters/edge_detection_filter.h"
#include "../filters/noise_reduction_filter.h"
#include "../engines/logger.h"
#include <boost/type_index.hpp>


SessionData::SessionData() : _batchFilter() {
    connect(&_batchFilter, &BatchFilter::onFinish, this, &SessionData::onBatchFilterApplied);
}

void SessionData::loadDirectory(const std::string &path) {
    _imageRepo.load_directory(path);
}

void SessionData::loadImage(const std::string &path) {
    _imageRepo.load_image(path);

    Image* selectedImage = _imageRepo.getImage();
    if (!selectedImage)
        return;

    emit loadActionList(selectedImage->getHistory());
}

void SessionData::applyFilter(std::unique_ptr<ImageFilter> filter) {
    Image* selectedImage = _imageRepo.getImage();
    if (!selectedImage){
        Logger::instance()->logMessage(
                    Logger::MessageTypes::error, Logger::MessageID::no_image_selected,
                    Logger::MessageOption::without_path,
                    {});
        return;
    }
    //check filter type
    if(dynamic_cast<GrayScaleFilter*>(filter.get())){
        const std::vector< std::unique_ptr<ImageState> > &states= selectedImage->getStates();
        for(auto it=states.begin();it!=states.end();it++){
            if((*it)->State==ImageStateSource::GrayScaleFilter){
                //TODO: make logger msg(filter already applied)
                Logger::instance()->logMessage(
                    Logger::MessageTypes::warning, Logger::MessageID::already_applied,
                    Logger::MessageOption::without_path,
                    {"Gray Scale"});
                return;
            }
        }
    }
    if(dynamic_cast<EdgeDetectionFilter*>(filter.get())){
        const std::vector< std::unique_ptr<ImageState> > &states= selectedImage->getStates();
        for(auto it=states.begin();it!=states.end();it++){
            if((*it)->State==ImageStateSource::EdgeDetectionFilter){
                //TODO: make logger msg(filter already applied)
                Logger::instance()->logMessage(
                    Logger::MessageTypes::warning, Logger::MessageID::already_applied,
                    Logger::MessageOption::without_path,
                    {"Edge Detection"});
                return;
            }
        }
    }
    if(dynamic_cast<NoiseReductionFilter*>(filter.get())){

    }
    
    cv::Mat filteredImage = filter->applyFilter(*selectedImage);
    selectedImage->setImage(&filteredImage, filter->getImageSource());

    emit updateActionList(selectedImage->getCurrentAction());
}

void SessionData::applyFilter(std::unique_ptr<ImageFilter> filter, std::vector<int> image_indices) {
    std::vector<Image*> batchInput = _imageRepo.getImages(std::move(image_indices));
    if (batchInput.empty())
        return;

    emit onBatchFilterStarted(batchInput.size());
    _batchFilter.apply(std::move(filter), batchInput);
}

void SessionData::onBatchFilterApplied(const std::vector<Image*> &input, const std::vector<cv::Mat> &output, ImageStateSource stateSource) {
    for (int i = 0; i < input.size(); ++i)
        input[i]->setImage((cv::Mat*)&output[i], stateSource);

    emit onBatchFilterFinished();
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
    return true; // TODO: return true or false?
}

bool SessionData::redo(){
    Image* selectedImage = _imageRepo.getImage();
    if (!selectedImage)
        //logger
        return false;


    if(selectedImage->redo()){
        emit updateActionList(selectedImage->getCurrentAction());
    }
    return true;
}

ImageRepository& SessionData::getImageRepo() {
    return _imageRepo;
}

Image* SessionData::getSelectedImage(void) {

    return _imageRepo.getImage();
}

void SessionData::accept(Visitor &v) const {
    v.visit(*this);
}

void SessionData::toggleHorizontalPlotMode()
{
    horizontalIntensityPlotMode = !horizontalIntensityPlotMode;    
}
