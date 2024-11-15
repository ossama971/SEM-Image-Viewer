#include "rightsidebarwidget.h"
#include "sharpen_filter_widget.h"
#include "controllerWidget.h"
#include "edge_extraction_wigdet.h"
#include "grayscalewidget.h"
#include "noisereductionwidget.h"
#include "historywidget.h"
#include "actionlistwidget.h"

RightSidebarWidget::RightSidebarWidget(QWidget *parent) : QWidget(parent), viewController(nullptr)
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
    HistoryWidget *_historyWidget = new HistoryWidget();
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
    _progressBar->setValue(_progressBar->value() + 1);
}

void RightSidebarWidget::hideProgressBar()
{
    _progressBar->hide();
}
