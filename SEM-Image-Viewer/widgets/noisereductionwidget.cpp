#include "noisereductionwidget.h"
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
    toggleButton->setArrowType(Qt::DownArrow);
    toggleButton->setCheckable(true);

    label->setText("Noise Reduction");

    QPushButton *applyButton = new QPushButton("Apply",this);
    //QIcon Icon("/absolute/path/to/icons/icons8-triangle-48.png");
    //applyButton->setIcon(Icon);
    //applyButton->setStyleSheet("border: none; background: transparent;");

    compactLayout->addWidget(toggleButton);
    compactLayout->addWidget(label);
    compactLayout->addWidget(applyButton);
    compactLayout->setSpacing(10);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(compactLayout);

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
        toggleButton->setArrowType(Qt::UpArrow);
        slidersWidget->show(); // Show sliders when checked
    }
    else {
        toggleButton->setArrowType(Qt::DownArrow);
        slidersWidget->hide(); // Hide sliders when unchecked
    }
}

void NoiseReductionWidget::handleApplyFilter()
{
    qDebug() << "Applying noise reduction filter";
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


