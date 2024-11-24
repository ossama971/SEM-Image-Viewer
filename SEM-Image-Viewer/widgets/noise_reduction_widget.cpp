#include "noise_reduction_widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

NoiseReductionWidget::NoiseReductionWidget(QWidget *parent)
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
        "padding:2px"
        "}"
        );
    label->setText("Noise Reduction");
    label->setStyleSheet("QLabel {background-color:transparent}");
    QPushButton *applyButton = new QPushButton(this);
    applyButton->setObjectName("applyButton");
    applyButton->setStyleSheet(
        "QPushButton { "
        "padding:2px"
        "}"
        );

  //  compactLayout->addWidget(toggleButton);
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
    IntensitySlider= new QSlider(Qt::Horizontal, this);

    IntensitySlider->setRange(0, 100);

    //slider low with label value
    QHBoxLayout *sliderlabelLayout = new QHBoxLayout();
    intensity= new QLabel();

    sliderlabelLayout->addWidget(IntensitySlider);
    sliderlabelLayout->addWidget(intensity);



    // Intensity slider with label
    QLabel *intensityLabel = new QLabel("Intensity", this);
    slidersLayout->addWidget(intensityLabel);
    slidersLayout->addLayout(sliderlabelLayout);


    slidersWidget->setLayout(slidersLayout);
    mainLayout->addWidget(slidersWidget);
    slidersWidget->hide(); // Initially hide the sliders

    setLayout(mainLayout);

    connect(toggleButton, &QToolButton::toggled, this, &NoiseReductionWidget::expandCollapseSliders);
    connect(applyButton,&QPushButton::clicked,this, &NoiseReductionWidget::handleApplyFilter);
    connect(IntensitySlider, &QSlider::valueChanged, this, &NoiseReductionWidget::handleIntensityChange);

}

void NoiseReductionWidget::expandCollapseSliders(bool checked)
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

void NoiseReductionWidget::handleApplyFilter()
{
    emit applyFilter();
}

void NoiseReductionWidget::handleIntensityChange(int value)
{
    intensity->setText(QString::number(value));
    emit intensityChanged(value);
}

int NoiseReductionWidget::getIntensity() const
{
    return IntensitySlider->value();
}


