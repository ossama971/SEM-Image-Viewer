#include "right_side_bar_widget.h"
#include "horizontal_intensity_plot.h"
#include "sharpen_filter_widget.h"
#include "controller_widget.h"
#include "edge_extraction_wigdet.h"
#include "grayscale_widget.h"
#include "noise_reduction_widget.h"
#include "history_widget.h"
#include "action_list_widget.h"
#include "heatmap_intensity_widget.h"
#include "../core/engines/workspace.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGuiApplication>
#include <QScreen>

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

    ActionListWidget *list=new ActionListWidget();
    list->addWidget(_sharpenWidget);
    list->addWidget(_grayScaleWidget);
    list->addWidget(_edgeExtractionWidget);
    list->addWidget(_noiseReductionWidget);
    rightSidebarLayout->setContentsMargins(10, 0, 0, 0);
    rightSidebarLayout->addWidget(list);

    HeatMapWidget *_heatmapWidget = new HeatMapWidget();
    horizontal_intensity_plot *_horizontal_intensity_widget = new horizontal_intensity_plot();

    OtherListWidget *otherList = new OtherListWidget();
    otherList->addWidget(_heatmapWidget);
    otherList->addWidget(_horizontal_intensity_widget);
    rightSidebarLayout->addWidget(otherList);


    rightSidebarLayout->addItem(new QSpacerItem(1, 2, QSizePolicy::Minimum, QSizePolicy::MinimumExpanding));

    rightSidebarLayout->addWidget(_historyWidget);

    controller.setEdgeExtractionWidget(_edgeExtractionWidget);
    controller.setSharpenWidget(_sharpenWidget);
    controller.setNoiseReductionWidget(_noiseReductionWidget);
    controller.setHistoryWidget(_historyWidget);
    controller.setGraySacleWidget(_grayScaleWidget);

    this->setLayout(rightSidebarLayout);

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

