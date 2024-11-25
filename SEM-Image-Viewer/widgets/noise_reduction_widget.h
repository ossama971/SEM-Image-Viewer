#ifndef NOISE_REDUCTION_WIDGET_H
#define NOISE_REDUCTION_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QToolButton>

class NoiseReductionWidget:public QWidget
{
    Q_OBJECT
public:
    explicit NoiseReductionWidget(QWidget* parent=nullptr);
    int getIntensity() const;

private:
    QLabel *label = nullptr;
    QToolButton *toggleButton = nullptr;
    QSlider *IntensitySlider = nullptr;
    QWidget *slidersWidget = nullptr;
    QLabel *intensity = nullptr;

signals:
    void intensityChanged(int value);
    void applyFilter();

private slots:
    void expandCollapseSliders(bool checked);
    void handleIntensityChange(int value);
    void handleApplyFilter();

};

#endif // NOISE_REDUCTION_WIDGET_H
