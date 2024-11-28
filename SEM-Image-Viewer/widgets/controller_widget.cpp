#include "controller_widget.h"
#include "image_widget.h"
#include "logger_widget.h"

#include "../core/filters/edge_detection_filter.h"
#include "../core/filters/noise_reduction_filter.h"
#include "../core/filters/sharpen_filter.h"
#include "../core/filters/gray_scale_filter.h"

#include "sharpen_filter_widget.h"
#include "edge_extraction_wigdet.h"
#include <opencv2/imgproc.hpp>

Controller::Controller() : SessionData_(Workspace::Instance()->getActiveSession()), gridView(nullptr) {
    SessionData_.loadImage("/home/bigfish/wsp/siemens/sem-image-viewer/SEM-Image-Viewer/assets/micro-electronic-sed.jpg");
}


void Controller::setHistoryWidget(HistoryWidget *widget)
{
    historyWidget = widget;
    if(historyWidget){
        connect(historyWidget,&HistoryWidget::redo,this, &Controller::redoAction);
        connect(historyWidget,&HistoryWidget::undo,this, &Controller::undoAction);
        connect(&SessionData_, &SessionData::loadActionList,historyWidget, &HistoryWidget::loadActionList);
        connect(&SessionData_.getImageRepo(), &ImageRepository::loadActionList,historyWidget, &HistoryWidget::loadActionList);
        connect(&SessionData_, &SessionData::updateActionList,historyWidget, &HistoryWidget::updateActionList);
        connect(&SessionData_, &SessionData::popActionList,historyWidget, &HistoryWidget::popAction);
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

void Controller::setSharpenWidget(SharpenWidget *widget)
{
    sharpenWidget = widget;
    if (sharpenWidget)
    {
        connect(sharpenWidget, &SharpenWidget::applyFilter, this, &Controller::onSharpenFilterApplied);
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
        connect(noiseReductionWidget, &NoiseReductionWidget::applyFilter, this, &Controller::onNoiseReductionFilterApplied);
    }
}
void Controller::setGraySacleWidget(GrayScaleWidget* widget)
{
    graySacleWidget=widget;
    if(graySacleWidget){
        connect(graySacleWidget,&GrayScaleWidget::applyFilter,this,&Controller::onGraySacleFilterApplied);
    }
}

void Controller::setGridView(GridView* widget) {
    gridView = widget;
}

// Slot to handle filter application
void Controller::onEdgeWidgetFilterApplied()
{
    int low = edgeExtractionWidget->getLowThreshold();
    int high = edgeExtractionWidget->getHighThreshold();

    std::unique_ptr<EdgeDetectionFilter> filter = std::make_unique<EdgeDetectionFilter>();
    filter->setThresholdLow(low);
    filter->setTHresholdHigh(high);

    applyFilter(std::move(filter));

}
void Controller::onNoiseReductionFilterApplied()
{
    int intensity=noiseReductionWidget->getIntensity();
    if (!intensity)
        return;
    std::unique_ptr<NoiseReductionFilter> filter = std::make_unique<NoiseReductionFilter>(intensity);

    applyFilter(std::move(filter));
}

void Controller::onSharpenFilterApplied()
{
    std::unique_ptr<SharpenFilter> filter = std::make_unique<SharpenFilter>();

    applyFilter(std::move(filter));
}
void Controller::onGraySacleFilterApplied()
{
    std::unique_ptr<GrayScaleFilter> filter = std::make_unique<GrayScaleFilter>();
    applyFilter(std::move(filter));
}

void Controller::applyFilter(std::unique_ptr<ImageFilter> filter) {
    if (gridView && gridView->isVisible())
    {
        std::vector<int> images = gridView->getSelectedImages();
        if (images.size() > 1)
        {
            SessionData_.applyFilter(std::move(filter), images);
            return;
        }
    }

    SessionData_.applyFilter(std::move(filter));
}

void Controller::undoAction(){

    if(SessionData_.undo()){

    }
}

void Controller::redoAction(){
    SessionData_.redo();
}


