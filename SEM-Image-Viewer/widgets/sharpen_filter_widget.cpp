#include "sharpen_filter_widget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QIcon>

SharpenWidget::SharpenWidget(QWidget *parent)
    : QWidget(parent),
    label(new QLabel(this))
{
    // Compact layout
    QHBoxLayout *compactLayout = new QHBoxLayout();
    label->setText("Sharpen");

    QPushButton *applyButton = new QPushButton(this);
    QIcon Icon(":/icons/play-icon-dark-mode.svg");
    applyButton->setIcon(Icon);
    applyButton->setStyleSheet(
        "QPushButton { "
        "padding:2px"
        "}"
        );
    // "QPushButton:hover { "
    // "   background-color: rgba(0, 122, 255, 0.2); " // Light blue highlight on hover
    // "}"
    compactLayout->addWidget(label);
    compactLayout->addWidget(applyButton);
    compactLayout->addStretch();
    compactLayout->setSpacing(5);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(compactLayout);



    setLayout(mainLayout);

    connect(applyButton,&QPushButton::clicked,this, &SharpenWidget::handleApplyFilter);

}


void SharpenWidget::handleApplyFilter()
{
    emit applyFilter();
}

