#ifndef EDGE_EXTRACTION_WIGDET_H
#define EDGE_EXTRACTION_WIGDET_H


#include <QWidget>
#include <QLabel>
#include <QToolButton>
#include <QStackedWidget>
#include <QSlider>


class EdgeExtractionWidget:public QWidget
{
    Q_OBJECT
public:
    explicit EdgeExtractionWidget(QWidget* parent=nullptr);
    int getLowThreshold() const;
    int getHighThreshold() const;

private:
    QLabel *label;
    QToolButton *toggleButton;
    QSlider *lowThresholdSlider;
    QSlider *highThresholdSlider;
    QWidget *slidersWidget;
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
