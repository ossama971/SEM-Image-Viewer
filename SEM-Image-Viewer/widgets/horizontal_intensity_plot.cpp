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
    applyButton = new QToolButton(this);
    applyButton->setObjectName("pen");
    applyButton->setCheckable(true);
    //Add label and Button to layout
    layout->addWidget(label);
    layout->addWidget(applyButton);
    layout->setContentsMargins(5,0,0,0);
    layout->addStretch();
    layout->setSpacing(5);

    setLayout(layout);


    connect(applyButton, &QToolButton::clicked,&Workspace::Instance()->getActiveSession(),&SessionData::toggleHorizontalPlotMode);

}

