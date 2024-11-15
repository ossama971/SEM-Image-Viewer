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
