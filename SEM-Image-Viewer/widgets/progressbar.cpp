#include "progressbar.h"

ProgressBarComponent::ProgressBarComponent(QWidget *parent) : QProgressBar(parent) {
    setTextVisible(true);
    setAlignment(Qt::AlignCenter);
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
