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
    toggleButton->setText("Options ...");
    toggleButton->setCheckable(true);
    toggleButton->setStyleSheet(
        "QToolButton { "
        "   border: none; "
        "   background-color: transparent; "
        "padding:2px"
        "}"
        "QToolButton:hover { "
        "   background-color: rgba(0, 122, 255, 0.2); " // Light blue highlight on hover
        "}"
        );

    label->setText("Edge Extraction");

    QPushButton *applyButton = new QPushButton(this);
    QIcon Icon(":/icons/play-icon.svg");
    applyButton->setIcon(Icon);
    applyButton->setStyleSheet(
        "QPushButton { "
        "   border: none; "
        "   background-color: transparent; "
        "padding:2px"
        "}"
        "QPushButton:hover { "
        "   background-color: rgba(0, 122, 255, 0.2); " // Light blue highlight on hover
        "}"
        );


    compactLayout->addWidget(label);
    compactLayout->addWidget(applyButton);
    compactLayout->addStretch();
    compactLayout->setSpacing(5);

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

    // Move toggleButton and slidersWidget to optionsContent
    optionsContentLayout->addWidget(toggleButton);
    optionsContentLayout->addWidget(slidersWidget);

    // Add line and content to options container
    optionsLayout->addWidget(line);
    optionsLayout->addWidget(optionsContent);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
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
        toggleButton->setText("");
        toggleButton->setArrowType(Qt::UpArrow);
        slidersWidget->show(); // Show sliders when checked
    }
    else {
        toggleButton->setText("Options ...");
        toggleButton->setArrowType(Qt::NoArrow);
        slidersWidget->hide(); // Hide sliders when unchecked
    }
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
