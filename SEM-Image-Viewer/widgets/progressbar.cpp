#include "progressbar.h"

ProgressBarComponent::ProgressBarComponent(QWidget *parent) : QWidget(parent) {
    progressBar = new QProgressBar(this);
    progressBar->setTextVisible(true);
    progressBar->setAlignment(Qt::AlignCenter);
    progressBar->setStyleSheet(
        "QProgressBar {"
        "    border: 2px solid #555;"
        "    border-radius: 10px;"
        "    background-color: #e0e0e0;"
        "    text-align: center;"
        "}"
        "QProgressBar::chunk {"
        "    background-color: #3C99D8;"  // Lighter blue color
        "    border-radius: 8px;"
        "}"
        "QProgressBar::text {"
        "    color: white;"
        "    font: bold 12px;"  // Change font size and make it bold
        "}"
        );
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(progressBar);
    setLayout(layout);
    hide();
}

void ProgressBarComponent::setMaxIterations(int max) {
    progressBar->setMaximum(max);
    resetProgress();
    show();
}

void ProgressBarComponent::incrementProgress() {
    progressBar->setValue(progressBar->value() + 1);
}

void ProgressBarComponent::resetProgress() {
    progressBar->setValue(0);
}

void ProgressBarComponent::hideProgressBar() {
    hide();
}
