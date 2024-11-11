#ifndef GRAYSCALEWIDGET_H
#define GRAYSCALEWIDGET_H

#include <QWidget>
#include <QLabel>
class GrayScaleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit GrayScaleWidget(QWidget *parent=nullptr);
private:
    QLabel *label;
signals:
    void applyFilter();
private slots:
    void handleApplyFilter();

};

#endif // GRAYSCALEWIDGET_H
