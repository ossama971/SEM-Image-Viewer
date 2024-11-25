#ifndef EDGE_EXTRACTION_WIGDET_H
#define EDGE_EXTRACTION_WIGDET_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QSlider>

class EdgeExtractionWidget:public QWidget
{
    Q_OBJECT
public:
    explicit EdgeExtractionWidget(QWidget* parent=nullptr);
    int getLowThreshold() const;
    int getHighThreshold() const;

private:
    QLabel *label = nullptr;
    QToolButton *toggleButton = nullptr;
    QSlider *lowThresholdSlider = nullptr;
    QSlider *highThresholdSlider = nullptr;
    QWidget *slidersWidget = nullptr;
    QLabel *low = nullptr;
    QLabel *high = nullptr;
signals:
    void thresholdLowChanged(int value);
    void thresholdHighChanged(int value);
    void applyFilter();

private slots:
    void expandCollapseSliders(bool checked);
    void handleLowThresholdChange(int value);
    void handleHighThresholdChange(int value);
    void handleApplyFilter();

};

#endif // EDGE_EXTRACTION_WIGDET_H
