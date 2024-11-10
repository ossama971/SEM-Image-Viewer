#include "progressbar.h"

ProgressBarComponent::ProgressBarComponent(QWidget *parent) : QProgressBar(parent) {
    setTextVisible(true);
    setAlignment(Qt::AlignCenter);
    setStyleSheet(
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

    // Remove unnecessary QVBoxLayout and hide directly
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
