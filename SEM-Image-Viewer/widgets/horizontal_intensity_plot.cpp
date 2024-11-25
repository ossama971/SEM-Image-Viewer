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
    QPushButton *applyButton = new QPushButton(this);
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
}
