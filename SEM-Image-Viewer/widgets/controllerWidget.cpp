#include "controllerWidget.h"
#include "image_widget.h"
#include "LoggerWidget.h"

#include "../core/filters/EdgeDetectionFilter.h"
#include "../core/filters/NoiseReductionFilter.h"
#include "../core/filters/SharpenFilter.h"
#include "../core/filters/GrayScaleFilter.h"

#include "sharpen_filter_widget.h"
#include "edge_extraction_wigdet.h"
#include <opencv2/imgproc.hpp>

Controller::Controller() : SessionData_(Workspace::Instance()->getActiveSession()) {
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
        connect(historyWidget,&HistoryWidget::redo,this, &Controller::redoAction);
        connect(historyWidget,&HistoryWidget::undo,this, &Controller::undoAction);
        connect(&SessionData_, &SessionData::loadActionList,historyWidget, &HistoryWidget::loadActionList);
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
// Slot to handle filter application
void Controller::onEdgeWidgetFilterApplied()
{
    int low = edgeExtractionWidget->getLowThreshold();
    int high = edgeExtractionWidget->getHighThreshold();

    std::unique_ptr<EdgeDetectionFilter> filter = std::make_unique<EdgeDetectionFilter>();
    filter->setThresholdLow(low);
    filter->setTHresholdHigh(high);
    SessionData_.applyFilter(std::move(filter));

}
void Controller::onNoiseReductionFilterApplied()
{
    int instensity=noiseReductionWidget->getIntensity();
    std::unique_ptr<NoiseReductionFilter> filter = std::make_unique<NoiseReductionFilter>(instensity);

    SessionData_.applyFilter(std::move(filter));
}

void Controller::onContourFilterApplied()
{
    std::unique_ptr<SharpenFilter> filter = std::make_unique<SharpenFilter>();

    SessionData_.applyFilter(std::move(filter));
}
void Controller::onGraySacleFilterApplied()
{
    std::unique_ptr<GrayScaleFilter> filter = std::make_unique<GrayScaleFilter>();
    SessionData_.applyFilter(std::move(filter));
}

void Controller::undoAction(){

    if(SessionData_.undo()){

    }
}

void Controller::redoAction(){
    SessionData_.redo();
}


