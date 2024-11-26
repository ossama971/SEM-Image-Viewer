#include "heat_map_widget.h"
#include "../core/engines/workspace.h"
#include <QHBoxLayout>
#include <QLabel>
HeatMapWidget::HeatMapWidget(QWidget *parent)
    :QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    // QLabel *label = new QLabel(this);
    // label->setText("Edge Extraction");
    // label->setStyleSheet("QLabel {background-color:transparent}");
    heatmapButton = new QCheckBox("Heat Map",this);
    heatmapButton->setCheckable(true);

    // auto buttonStyle =  "QToolButton {  border: none; }"
    //                     "QToolButton:checked { padding: 0px 3px 0px 3px;}"
    //                     "QToolButton:hover { padding: 0px 3px 0px 3px;}";
    //heatmapButton->setStyleSheet(buttonStyle);

    layout->setContentsMargins(0,10,0,10);
    layout->addWidget(heatmapButton);
    setLayout(layout);

    connect(heatmapButton,&QCheckBox::toggled,this,&HeatMapWidget::handleApplyHeatMap);
    connect(&Workspace::Instance()->getActiveSession().getImageRepo(), &ImageRepository::onImageChanged, this,&HeatMapWidget::resetToggle);
}
void HeatMapWidget::handleApplyHeatMap(bool checked)
{
    Image *selected_image=Workspace::Instance()->getActiveSession().getImageRepo().getImage();
    if(!selected_image)
    {
        heatmapButton->setChecked(false);
        return;
    }
    cv::Mat heatmap=Utils::heatmap(selected_image->getImageMat());
    emit applyHeatMap(heatmap,checked);
}
void HeatMapWidget::resetToggle()
{
    heatmapButton->blockSignals(true);
    heatmapButton->setChecked(false);
    heatmapButton->blockSignals(false);

    Image* newImage = Workspace::Instance()->getActiveSession().getImageRepo().getImage();
    if (newImage) {
        connect(newImage, &Image::onImageStateUpdated, this, &HeatMapWidget::resetToggle);
    }
}

