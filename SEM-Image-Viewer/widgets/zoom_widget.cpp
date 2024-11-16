#include "zoom_widget.h"
#include <QHBoxLayout>
#include <QIcon>
#include <QDir>
ZoomWidget::ZoomWidget(QWidget *parent)
    : QWidget(parent), zoomInButton(new QPushButton(this)), zoomOutButton(new QPushButton(this))
{
    setFixedSize(70, 40);
    setupLayout();
}

void ZoomWidget::setupLayout()
{
    QHBoxLayout *layout = new QHBoxLayout(this);

    // Set icons for zoom buttons
    QIcon zoomInIcon(":/icons/zoom-in-dark-mode.svg");
    QIcon zoomOutIcon(":/icons/zoom-out-dark-mode.svg");

    zoomInButton->setObjectName("zoom-in");
    zoomOutButton->setObjectName("zoom-out");
    zoomInButton->setCursor(Qt::PointingHandCursor);
    zoomOutButton->setCursor(Qt::PointingHandCursor);

    zoomInButton->setStyleSheet("border: none; background: transparent;");
    zoomOutButton->setStyleSheet("border: none; background: transparent;");


    // Add buttons to layout
    layout->addWidget(zoomInButton);
    layout->addWidget(zoomOutButton);
    layout->setSpacing(5);
    setLayout(layout);

    // Connect buttons to widget signals
    connect(zoomInButton, &QPushButton::clicked, this, &ZoomWidget::zoomInRequested);
    connect(zoomOutButton, &QPushButton::clicked, this, &ZoomWidget::zoomOutRequested);
}
