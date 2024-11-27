#ifndef HORIZONTAL_INTENSITY_PLOT_H
#define HORIZONTAL_INTENSITY_PLOT_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>
class horizontal_intensity_plot: public QWidget
{
    Q_OBJECT
public:
    explicit horizontal_intensity_plot(QWidget *parent=nullptr);
private:
    QLabel *label = nullptr;
    QToolButton *applyButton = nullptr;



};

#endif // HORIZONTAL_INTENSITY_PLOT_H
