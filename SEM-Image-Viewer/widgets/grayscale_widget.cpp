#include "grayscale_widget.h"

#include <QHBoxLayout>
#include <QPushButton>

GrayScaleWidget::GrayScaleWidget(QWidget *parent)
    :QWidget(parent),
    label(new QLabel(this))
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    label->setText("Gray Scale");
    label->setStyleSheet("QLabel {background-color:transparent}");
    QPushButton *applyButton = new QPushButton(this);
    applyButton->setObjectName("applyButton");
    applyButton->setToolTip("Apply");
    applyButton->setStyleSheet(
        "QPushButton { "
        "padding:2px"
        "}"
        );
    //Add label and Button to layout
    layout->setContentsMargins(0,0,0,0);
    layout->addWidget(label);
    layout->addWidget(applyButton);
    layout->addStretch();
    layout->setSpacing(5);

    setLayout(layout);


    connect(applyButton,&QPushButton::clicked,this,&GrayScaleWidget::handleApplyFilter);
}
void GrayScaleWidget::handleApplyFilter()
{
    emit applyFilter();
}
