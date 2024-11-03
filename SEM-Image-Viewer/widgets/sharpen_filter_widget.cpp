#include "sharpen_filter_widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QIcon>

ContourWidget::ContourWidget(QWidget *parent)
    : QWidget(parent),
    label(new QLabel(this))
{
    // Compact layout
    QHBoxLayout *compactLayout = new QHBoxLayout();
    label->setText("Sharpen");

    QPushButton *applyButton = new QPushButton(this);
    applyButton->setText("Apply");
    compactLayout->addWidget(label);
    compactLayout->addWidget(applyButton);
    compactLayout->setSpacing(10);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(compactLayout);



    setLayout(mainLayout);

    connect(applyButton,&QPushButton::clicked,this, &ContourWidget::handleApplyFilter);

}


void ContourWidget::handleApplyFilter()
{
    emit applyFilter();
}

