#include "heat_map_widget.h"
#include "../core/engines/workspace.h"
#include <QHBoxLayout>

HeatMapWidget::HeatMapWidget(QWidget *parent)
    :QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    heatmapButton = new QToolButton(this);
    heatmapButton->setText("Heat Map");
    heatmapButton->setCheckable(true);

    auto buttonStyle =  "QToolButton { margin: 0px; padding: 2px 5px 2px 5px; border: none; }"
                        "QToolButton:checked { border-radius: 4px; padding: 0px 3px 0px 3px;}"
                        "QToolButton:hover { border-radius: 4px; padding: 0px 3px 0px 3px;}";
    heatmapButton->setStyleSheet(buttonStyle);

    layout->addWidget(heatmapButton);
    layout->setAlignment(Qt::AlignLeft);
    setLayout(layout);

    connect(heatmapButton,&QToolButton::toggled,this,&HeatMapWidget::handleApplyHeatMap);
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

