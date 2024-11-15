#include "grayscalewidget.h"

#include <QHBoxLayout>
#include <QPushButton>

GrayScaleWidget::GrayScaleWidget(QWidget *parent)
    :QWidget(parent),
    label(new QLabel(this))
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    label->setText("Gray Scale");

    QPushButton *applyButton = new QPushButton(this);
    applyButton->setObjectName("applyButton");
    applyButton->setStyleSheet(
        "QPushButton { "
        "padding:2px"
        "}"
        );
    //Add label and Button to layout
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
