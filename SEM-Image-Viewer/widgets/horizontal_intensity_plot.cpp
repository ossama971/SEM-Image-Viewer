#include "horizontal_intensity_plot.h"
#include "../core/engines/workspace.h"
#include <QHBoxLayout>
#include <QPushButton>

horizontal_intensity_plot::horizontal_intensity_plot(QWidget * parent)
    :QWidget(parent),
    label(new QLabel(this))
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    label->setText("Intensity Plot");
    label->setStyleSheet("QLabel {background-color:transparent}");
    applyButton = new QPushButton(this);
    QPixmap pixmap(":/icons/pen-icon.svg");
    QIcon ButtonIcon(pixmap);
    applyButton->setIcon(ButtonIcon);
    applyButton->setStyleSheet(
        "QPushButton { "
        "padding:2px"
        "}"
        );
    //Add label and Button to layout
    layout->addWidget(label);
    layout->addWidget(applyButton);
    layout->setContentsMargins(5,0,0,0);
    layout->addStretch();
    layout->setSpacing(5);

    setLayout(layout);


    connect(applyButton, &QPushButton::clicked,&Workspace::Instance()->getActiveSession(),&SessionData::toggleHorizontalPlotMode);
    connect(applyButton, &QPushButton::clicked, this, &horizontal_intensity_plot::toggleStyleSheet);
}


void horizontal_intensity_plot::toggleStyleSheet()
{
    if(Workspace::Instance()->getActiveSession().horizontalIntensityPlotMode)
    {
        applyButton->setStyleSheet("QPushButton {"
                                   "background-color: rgba( 55,55,55,0.5);"  // Example color for toggle
                                   "border-radius: 4px;"
                                   "}");
    }
    else {
        applyButton->setStyleSheet("QPushButton {"
                                   "background-color: transparent"
                                   "border-radius: 4px;"
                                   "}");  // Example color for toggle
    }

}
