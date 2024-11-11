#include "controllerWidget.h"
#include "image_widget.h"
#include "LoggerWidget.h"

#include "../core/filters/EdgeDetectionFilter.h"
#include "../core/filters/NoiseReductionFilter.h"
#include "../core/filters/SharpenFilter.h"

#include "sharpen_filter_widget.h"
#include "edge_extraction_wigdet.h"
#include <opencv2/imgproc.hpp>

Controller::Controller() : SessionData_(Workspace::Instance().getActiveSession()) {
    SessionData_.loadImage("/home/bigfish/wsp/siemens/sem-image-viewer/SEM-Image-Viewer/assets/micro-electronic-sed.jpg");
}

void Controller::setLoggerWidget(LoggerWidget *widget)
{
    loggerWidget = widget;
}
void Controller::setHistoryWidget(HistoryWidget *widget)
{
    historyWidget = widget;
    if(historyWidget){
        //connect(historyWidget,&HistoryWidget::redo,this, &Controller::redoAction);
        //connect(historyWidget,&HistoryWidget::undo,this, &Controller::undoAction);
    }
}

void Controller::setImageWidget(ImageWidget *widget)
{
    imageWidget = widget;
    if (imageWidget)
    {
        connect(this, &Controller::imageUpdated, imageWidget, &ImageWidget::updateImage);
    }
}

void Controller::setContourWidget(ContourWidget *widget)
{
    contourWidget = widget;
    if (contourWidget)
    {
        connect(contourWidget, &ContourWidget::applyFilter, this, &Controller::onContourFilterApplied);
    }
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
void Controller::setNoiseReductionWidget(NoiseReductionWidget *widget)
{
    noiseReductionWidget = widget;
    if (noiseReductionWidget)
    {
        // Connect the EdgeExtractionWidget's signal to the controller's slot
        // connect(noiseReductionWidget, &NoiseReductionWidget::applyFilter, this, &Controller::onNoiseReductionFilterApplied);
    }
}

// Slot to handle filter application
void Controller::onEdgeWidgetFilterApplied()
{
    // int low = edgeExtractionWidget->getLowThreshold();
    // int high = edgeExtractionWidget->getHighThreshold();
    //
    // std::unique_ptr<EdgeDetectionFilter> filter = std::make_unique<EdgeDetectionFilter>();
    // filter->setThresholdLow(low);
    // filter->setTHresholdHigh(high);
    //
    //
    //
    // auto updatedImage = ImageSession_.applyFilter(std::move(filter));
    // ImageSession_.getImage().setMat(updatedImage);
    //
    // emit imageUpdated(updatedImage);

    // loggerWidget->addLogMessage("Info", "filter Applied");
}
void Controller::onNoiseReductionFilterApplied()
{
    std::unique_ptr<NoiseReductionFilter> filter = std::make_unique<NoiseReductionFilter>(13.0f);

    SessionData_.applyFilter(std::move(filter));
}

void Controller::onContourFilterApplied()
{
    std::unique_ptr<SharpenFilter> filter = std::make_unique<SharpenFilter>();

    SessionData_.applyFilter(std::move(filter));
}

void Controller::printMat(const cv::Mat &mat)
{
    std::cout << "Matrix Size: " << mat.rows << " x " << mat.cols << std::endl;

    // Check if the matrix is empty
    if (mat.empty())
    {
        std::cout << "Matrix is empty!" << std::endl;
        return;
    }

    std::cout << "Matrix Data: ";
    for (int i = 0; i < std::min(mat.rows, 5); ++i)
    { // Print first 5 rows
        for (int j = 0; j < std::min(mat.cols, 5); ++j)
        { // Print first 5 columns
            if (mat.channels() == 1)
            {
                std::cout << static_cast<int>(mat.at<uchar>(i, j)) << " ";
            }
            else if (mat.channels() == 3)
            {
                cv::Vec3b pixel = mat.at<cv::Vec3b>(i, j);
                std::cout << "(" << static_cast<int>(pixel[0]) << ", "
                          << static_cast<int>(pixel[1]) << ", "
                          << static_cast<int>(pixel[2]) << ") ";
            }
        }
        std::cout << std::endl; // New line for each row
    }
}
