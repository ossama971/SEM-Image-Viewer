#include "edge_extraction_wigdet.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QFrame>
#include <QStyle>

EdgeExtractionWidget::EdgeExtractionWidget(QWidget *parent)
    : QWidget(parent),
    toggleButton(new QToolButton(this)),
    label(new QLabel(this)),
    slidersWidget(new QWidget(this))
{
    // Compact layout
    QHBoxLayout *compactLayout = new QHBoxLayout();
    toggleButton->setObjectName("down");
    toggleButton->setCheckable(true);
    toggleButton->setStyleSheet(
        "QToolButton {"
        "    border: none;"
        "    font-size: 12px;"  // Adjust font size as needed
        "}"
        );
    label->setText("Edge Extraction");
    label->setStyleSheet("QLabel {background-color:transparent}");
    QPushButton *applyButton = new QPushButton(this);
    applyButton->setObjectName("applyButton");
    applyButton->setToolTip("Apply");
    applyButton->setStyleSheet(
        "QPushButton { "
        "padding:2px"
        "}"
        );


    compactLayout->addWidget(label);
    compactLayout->addWidget(applyButton);
    compactLayout->addStretch();
    compactLayout->setSpacing(5);
    compactLayout->addWidget(toggleButton);

    QWidget *optionsContainer = new QWidget(this);
    QHBoxLayout *optionsLayout = new QHBoxLayout(optionsContainer);
    optionsLayout->setContentsMargins(0, 0, 0, 0);

    // Add vertical line
    QFrame *line = new QFrame(optionsContainer);
    line->setFrameShape(QFrame::VLine);
    line->setFrameShadow(QFrame::Plain);
    line->setStyleSheet("color: #404040;");

    // Create container for options content
    QWidget *optionsContent = new QWidget(optionsContainer);
    QVBoxLayout *optionsContentLayout = new QVBoxLayout(optionsContent);
    optionsContentLayout->setContentsMargins(10, 0, 0, 0);

    // Move slidersWidget to optionsContent
    optionsContentLayout->addWidget(slidersWidget);

    // Add line and content to options container
    optionsLayout->addWidget(line);
    optionsLayout->addWidget(optionsContent);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,10,0,10);
    mainLayout->addLayout(compactLayout);
    mainLayout->addWidget(optionsContainer);

    // Sliders layout
    QVBoxLayout *slidersLayout = new QVBoxLayout();
    lowThresholdSlider = new QSlider(Qt::Horizontal, this);
    highThresholdSlider = new QSlider(Qt::Horizontal, this);

    lowThresholdSlider->setRange(0, 255);
    highThresholdSlider->setRange(0, 255);

    //slider low with label value
    QHBoxLayout *sliderlabelLayoutLow = new QHBoxLayout();
    low = new QLabel();

    sliderlabelLayoutLow->addWidget(lowThresholdSlider);
    sliderlabelLayoutLow->addWidget(low);

    //slider high with label value
    QHBoxLayout *sliderlabelLayoutHigh = new QHBoxLayout();
    high= new QLabel();

    sliderlabelLayoutHigh->addWidget(highThresholdSlider);
    sliderlabelLayoutHigh->addWidget(high);


    // Low threshold slider with label
    QLabel *lowLabel = new QLabel("Low Threshold", this);
    slidersLayout->addWidget(lowLabel);
    slidersLayout->addLayout(sliderlabelLayoutLow);

    // High threshold slider with label
    QLabel *highLabel = new QLabel("High Threshold", this);
    slidersLayout->addWidget(highLabel);
    slidersLayout->addLayout(sliderlabelLayoutHigh);

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
         toggleButton->setObjectName("up");
        slidersWidget->show(); // Show sliders when checked
    }
    else {       
         toggleButton->setObjectName("down");
        slidersWidget->hide(); // Hide sliders when unchecked
    }
    toggleButton->style()->unpolish(toggleButton);
    toggleButton->style()->polish(toggleButton);
    toggleButton->update();
}

void EdgeExtractionWidget::handleApplyFilter()
{
    emit applyFilter();
}

void EdgeExtractionWidget::handleLowThresholdChange(int value)
{
    low->setText(QString::number(value));
    emit thresholdLowChanged(value);
}

void EdgeExtractionWidget::handleHighThresholdChange(int value)
{
    high->setText(QString::number(value));
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
