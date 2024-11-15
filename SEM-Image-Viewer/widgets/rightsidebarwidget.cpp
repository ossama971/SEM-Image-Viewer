#include "rightsidebarwidget.h"
#include "sharpen_filter_widget.h"
#include "controllerWidget.h"
#include "edge_extraction_wigdet.h"
#include "grayscalewidget.h"
#include "noisereductionwidget.h"
#include "historywidget.h"
#include "actionlistwidget.h"
#include "../core/engines/Workspace.h"

RightSidebarWidget::RightSidebarWidget(QWidget *parent) : QWidget(parent), viewController(nullptr), _imageRepo(&Workspace::Instance()->getActiveSession().getImageRepo())
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
    ContourWidget *_contourWidget = new ContourWidget();
    EdgeExtractionWidget *_edgeExtractionWidget = new EdgeExtractionWidget();
    GrayScaleWidget *_grayScaleWidget = new GrayScaleWidget();
    NoiseReductionWidget *_noiseReductionWidget = new NoiseReductionWidget();
    HistoryWidget *_historyWidget = new HistoryWidget();
    _progressBar = new ProgressBarComponent();

    ActionListWidget *list=new ActionListWidget();
    list->addWidget(_contourWidget);
    list->addWidget(_grayScaleWidget);
    list->addWidget(_edgeExtractionWidget);
    list->addWidget(_noiseReductionWidget);
    // rightSidebarLayout->addWidget(_contourWidget);
    // rightSidebarLayout->addWidget(_grayScaleWidget);
    // rightSidebarLayout->addWidget(_edgeExtractionWidget);
    // rightSidebarLayout->addWidget(_noiseReductionWidget);
    rightSidebarLayout->addWidget(list);
    rightSidebarLayout->addWidget(_progressBar);

    rightSidebarLayout->addWidget(_historyWidget);

    controller.setEdgeExtractionWidget(_edgeExtractionWidget);
    controller.setContourWidget(_contourWidget);
    controller.setNoiseReductionWidget(_noiseReductionWidget);
    controller.setHistoryWidget(_historyWidget);
    controller.setGraySacleWidget(_grayScaleWidget);

    this->setLayout(rightSidebarLayout);

    connect(_imageRepo, &ImageRepository::onImageLoadStarted, this, &RightSidebarWidget::onImageLoadStarted);
    connect(_imageRepo, &ImageRepository::onImageLoaded, this, &RightSidebarWidget::onImageLoaded);
}

void RightSidebarWidget::setViewController(WidgetViewController* widgetViewController) {
    viewController = widgetViewController;
}

void RightSidebarWidget::setMaxMinWidth(int mn, int mx)
{
    setMinimumWidth(mn);
    setMaximumWidth(mx);
}

void RightSidebarWidget::initializeProgress(int maxIterations)
{
    _progressBar->setMaximum(maxIterations);
    _progressBar->setValue(0);
    _progressBar->show();
}

void RightSidebarWidget::updateProgress()
{
    int new_value = _progressBar->value() + 1;
    _progressBar->setValue(new_value);

    if (new_value == _progressBar->maximum())
        hideProgressBar();
}

void RightSidebarWidget::hideProgressBar()
{
    _progressBar->hide();
}

void RightSidebarWidget::onImageLoadStarted(int image_count) {
    initializeProgress(image_count);
}

void RightSidebarWidget::onImageLoaded(Image* newImage) {
    updateProgress();
}
