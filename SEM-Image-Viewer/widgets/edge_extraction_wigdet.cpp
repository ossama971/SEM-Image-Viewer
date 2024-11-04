#include "edge_extraction_wigdet.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QIcon>

EdgeExtractionWidget::EdgeExtractionWidget(QWidget *parent)
    : QWidget(parent),
    toggleButton(new QToolButton(this)),
    label(new QLabel(this)),
    slidersWidget(new QWidget(this))
{
    // Compact layout
    QHBoxLayout *compactLayout = new QHBoxLayout();
    toggleButton->setArrowType(Qt::DownArrow);
    toggleButton->setCheckable(true);

    label->setText("Contour");

    QPushButton *applyButton = new QPushButton(this);
    //QIcon Icon("/absolute/path/to/icons/icons8-triangle-48.png");
    //applyButton->setIcon(Icon);
    //applyButton->setStyleSheet("border: none; background: transparent;");
    applyButton->setText("Apply");

    compactLayout->addWidget(toggleButton);
    compactLayout->addWidget(label);
    compactLayout->addWidget(applyButton);
    compactLayout->setSpacing(10);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(compactLayout);

    // Sliders layout
    QVBoxLayout *slidersLayout = new QVBoxLayout();
    lowThresholdSlider = new QSlider(Qt::Horizontal, this);
    highThresholdSlider = new QSlider(Qt::Horizontal, this);

    lowThresholdSlider->setRange(0, 255);
    highThresholdSlider->setRange(0, 255);

    // Low threshold slider with label
    QLabel *lowLabel = new QLabel("Low Threshold", this);
    slidersLayout->addWidget(lowLabel);
    slidersLayout->addWidget(lowThresholdSlider);

    // High threshold slider with label
    QLabel *highLabel = new QLabel("High Threshold", this);
    slidersLayout->addWidget(highLabel);
    slidersLayout->addWidget(highThresholdSlider);

    slidersWidget->setLayout(slidersLayout);
    mainLayout->addWidget(slidersWidget);
    slidersWidget->hide(); // Initially hide the sliders

    setLayout(mainLayout);

    connect(toggleButton, &QToolButton::toggled, this, &EdgeExtractionWidget::expandCollapseSliders);
    connect(applyButton,&QPushButton::clicked,this, &EdgeExtractionWidget::handleApplyFilter);
    connect(lowThresholdSlider, &QSlider::valueChanged, this, &EdgeExtractionWidget::handleLowThresholdChange);
    connect(highThresholdSlider, &QSlider::valueChanged, this, &EdgeExtractionWidget::handleHighThresholdChange);
}

void EdgeExtractionWidget::expandCollapseSliders(bool checked)
{
    if (checked) {
        toggleButton->setArrowType(Qt::UpArrow);
        slidersWidget->show(); // Show sliders when checked
    }
    else {
        toggleButton->setArrowType(Qt::DownArrow);
        slidersWidget->hide(); // Hide sliders when unchecked
    }
}

void EdgeExtractionWidget::handleApplyFilter()
{
    emit applyFilter();
}

void EdgeExtractionWidget::handleLowThresholdChange(int value)
{
    emit thresholdLowChanged(value);
}

void EdgeExtractionWidget::handleHighThresholdChange(int value)
{
    emit thresholdHighChanged(value);
}
int EdgeExtractionWidget::getLowThreshold() const
{
    return lowThresholdSlider->value();
}
int EdgeExtractionWidget::getHighThreshold() const
{
    return highThresholdSlider->value();
}
