#include "rightsidebarwidget.h"
#include "sharpen_filter_widget.h"
#include "controllerWidget.h"
#include "edge_extraction_wigdet.h"
#include "grayscalewidget.h"
#include "noisereductionwidget.h"
#include "historywidget.h"
#include "actionlistwidget.h"
#include "../core/engines/Workspace.h"

RightSidebarWidget::RightSidebarWidget(QWidget *parent) : QWidget(parent), _imageRepo(&Workspace::Instance()->getActiveSession().getImageRepo())
{
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    // setStyleSheet("background-color: #000000;");
    setMinimumWidth(mainScreenWidth * 0.05);
    setMaximumWidth(mainScreenWidth * 0.3);
    Controller &controller = Controller::instance();
    QVBoxLayout *rightSidebarLayout = new QVBoxLayout();

    // QWidget *rightContent = new QWidget(parent);
    // rightContent->setStyleSheet("background-color: #627e7c;");
    SharpenWidget *_sharpenWidget = new SharpenWidget();
    EdgeExtractionWidget *_edgeExtractionWidget = new EdgeExtractionWidget();
    GrayScaleWidget *_grayScaleWidget = new GrayScaleWidget();
    NoiseReductionWidget *_noiseReductionWidget = new NoiseReductionWidget();
    _historyWidget = new HistoryWidget();
    _progressBar = new ProgressBarComponent();

    ActionListWidget *list=new ActionListWidget();
    list->addWidget(_sharpenWidget);
    list->addWidget(_grayScaleWidget);
    list->addWidget(_edgeExtractionWidget);
    list->addWidget(_noiseReductionWidget);
    // rightSidebarLayout->addWidget(_contourWidget);
    // rightSidebarLayout->addWidget(_grayScaleWidget);
    // rightSidebarLayout->addWidget(_edgeExtractionWidget);
    // rightSidebarLayout->addWidget(_noiseReductionWidget);
    rightSidebarLayout->addWidget(list);

    QWidget *progressBarContainer = new QWidget();
    QHBoxLayout *progressBarLayout = new QHBoxLayout();
    progressBarLayout->addWidget(_progressBar);
    progressBarContainer->setLayout(progressBarLayout);
    rightSidebarLayout->addWidget(progressBarContainer);
    progressBarContainer->setContentsMargins(3, 0, 3, 0);
    rightSidebarLayout->addWidget(_historyWidget);

    controller.setEdgeExtractionWidget(_edgeExtractionWidget);
    controller.setSharpenWidget(_sharpenWidget);
    controller.setNoiseReductionWidget(_noiseReductionWidget);
    controller.setHistoryWidget(_historyWidget);
    controller.setGraySacleWidget(_grayScaleWidget);

    this->setLayout(rightSidebarLayout);

    connect(_imageRepo, &ImageRepository::onImageLoadStarted, this, &RightSidebarWidget::onImageLoadStarted);
    connect(_imageRepo, &ImageRepository::onImageLoaded, this, &RightSidebarWidget::onImageLoaded);
}

void RightSidebarWidget::setVisible(bool visible) {
    QWidget::setVisible(visible);

    emit onVisibilityChange(visible);
}

void RightSidebarWidget::setMaxMinWidth(int mn, int mx)
{
    setMinimumWidth(mn);
    setMaximumWidth(mx);
}

void RightSidebarWidget::initializeProgress(int maxIterations)
{
    std::lock_guard<std::mutex> guard(_progressBarMtx);

    _progressBar->setMaximum(maxIterations);
    _progressBar->setValue(0);
    _progressBar->show();
}

void RightSidebarWidget::updateProgress()
{
    std::unique_lock<std::mutex> guard(_progressBarMtx);

    int new_value = _progressBar->value() + 1;
    _progressBar->setValue(new_value);

    if (new_value == _progressBar->maximum())
    {
        //guard.unlock();
        //hideProgressBar();
        _progressBar->hide();
    }
}

void RightSidebarWidget::hideProgressBar()
{
    std::lock_guard<std::mutex> guard(_progressBarMtx);

    _progressBar->hide();
}

void RightSidebarWidget::onImageLoadStarted(int image_count) {
    initializeProgress(image_count);
}

void RightSidebarWidget::onImageLoaded(Image* newImage) {
    updateProgress();
}
