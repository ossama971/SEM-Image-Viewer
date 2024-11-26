#ifndef HEATMAP_INTENSITY_WIDGET_H
#define HEATMAP_INTENSITY_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

class HeatIntensityWidget : public QWidget {
    Q_OBJECT
public:
    explicit HeatIntensityWidget(QWidget* parent = nullptr);

    // Method to add widgets to the container
    void addWidget(QWidget* widget);

private:
    QVBoxLayout* m_itemsLayout = nullptr;
};

#endif // HEATMAP_INTENSITY_WIDGET_H
