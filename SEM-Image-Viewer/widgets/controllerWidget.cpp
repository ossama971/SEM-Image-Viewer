#include "controllerWidget.h"
#include "image_widget.h"
#include "LoggerWidget.h"
#include "../core/filters/EdgeDetectionFilter.h"
#include "sharpen_filter_widget.h"
#include "edge_extraction_wigdet.h"
#include <opencv2/imgproc.hpp>

Controller::Controller():ImageSession(Workspace::Instance().getActiveSession()) {

};

void Controller::setLoggerWidget(LoggerWidget *widget)
{
    loggerWidget = widget;

}

void Controller::setImageWidget(ImageWidget *widget)
{
    imageWidget = widget;
    if(imageWidget){
        connect(this, &Controller::imageUpdated, imageWidget, &ImageWidget::updateImage);
    }
}

void Controller::setContourWidget(ContourWidget *widget)
{
    contourWidget = widget;
}

void Controller::setEdgeExtractionWidget(EdgeExtractionWidget *widget)
{
    edgeExtractionWidget = widget;
    if (edgeExtractionWidget)
    {
        // Connect the EdgeExtractionWidget's signal to the controller's slot
        connect(edgeExtractionWidget, &EdgeExtractionWidget::applyFilter, this, &Controller::onEdgeWidgetFilterApplied);
    }
}

// Slot to handle filter application
void Controller::onEdgeWidgetFilterApplied()
{

    int low=edgeExtractionWidget->getLowThreshold();
    int hight=edgeExtractionWidget->getHighThreshold();
    //EdgeDetectionFilter * _edgeDetectionFilter
//    auto updatedImage=ImageSession.applyFilter(imageWidget->getImage());


   // emit imageUpdated(updatedImage);
    loggerWidget->addLogMessage("Error","filter Applied");
}

void Controller::onContourFilterApplied()
{
}

void Controller::printMat(const cv::Mat &mat) {
    std::cout << "Matrix Size: " << mat.rows << " x " << mat.cols << std::endl;

    // Check if the matrix is empty
    if (mat.empty()) {
        std::cout << "Matrix is empty!" << std::endl;
        return;
    }

    std::cout << "Matrix Data: ";
    for (int i = 0; i < std::min(mat.rows, 5); ++i) { // Print first 5 rows
        for (int j = 0; j < std::min(mat.cols, 5); ++j) { // Print first 5 columns
            if (mat.channels() == 1) {
                std::cout << static_cast<int>(mat.at<uchar>(i, j)) << " ";
            } else if (mat.channels() == 3) {
                cv::Vec3b pixel = mat.at<cv::Vec3b>(i, j);
                std::cout << "(" << static_cast<int>(pixel[0]) << ", "
                          << static_cast<int>(pixel[1]) << ", "
                          << static_cast<int>(pixel[2]) << ") ";
            }
        }
        std::cout << std::endl; // New line for each row
    }
}
