#ifndef CONTOUR_WIDGET_H
#define CONTOUR_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QToolButton>



class SharpenWidget:public QWidget
{
    Q_OBJECT
public:
    explicit SharpenWidget(QWidget* parent=nullptr);


private:
    QLabel *label = nullptr;
    QToolButton *toggleButton = nullptr;
    QWidget *slidersWidget = nullptr;
signals:
    void applyFilter();

private slots:
    void handleApplyFilter();

};

#endif
