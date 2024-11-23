#include "progressbar.h"

ProgressBarComponent::ProgressBarComponent(QWidget *parent) : QProgressBar(parent)
{
    setTextVisible(true);
    setAlignment(Qt::AlignCenter);

    setStyleSheet(
        "QProgressBar {"
        "    background-color: #e6e6e6;" // Light gray background
        "    border: 1px solid #b0b0b0;"
        "    border-radius: 3px;"
        "    height: 6px;"
        "    text-align: center;"

        "    font: normal 10px Arial;"
        "}"
        "QProgressBar::chunk {"

        "    background-color: #3C99D8;" // Lighter blue color
        "    border-radius: 8px;"
        "}");
}

void ProgressBarComponent::setMaxIterations(int max)
{
    setMaximum(max);
    resetProgress();
    show();
}

void ProgressBarComponent::incrementProgress()
{
    int new_value = value() + 1;
    setValue(new_value);
    if (new_value == maximum())
    {
        ;
        hide();
    }
}

void ProgressBarComponent::resetProgress()
{
    setValue(0);
}

void ProgressBarComponent::hideProgressBar()
{
    hide();
}
