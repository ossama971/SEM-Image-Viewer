#include "logCardWidget.h"

LogCard::LogCard(const QString &headerText, const QString &detailsText, QWidget *parent)
    : QWidget(parent), detailsVisible(false)
{
    // Initialize widgets
    headerLabel = new QLabel(headerText, this);
    detailsLabel = new QLabel(detailsText, this);
    detailsLabel->setWordWrap(true);
    detailsLabel->setVisible(false);

    detailsBtn = new QPushButton("Show Details", this);

    // Layout setup
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(headerLabel);
    layout->addWidget(detailsLabel);
    layout->addWidget(detailsBtn);
    setLayout(layout);

    // Connect button to slot
    connect(detailsBtn, &QPushButton::clicked, this, &LogCard::toggleDetails);
}

void LogCard::toggleDetails()
{
    // Toggle visibility
    detailsVisible = !detailsVisible;
    detailsLabel->setVisible(detailsVisible);

    // Update button text
    detailsBtn->setText(detailsVisible ? "Hide Details" : "Show Details");
}
