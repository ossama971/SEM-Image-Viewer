#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

#include <QWidget>
#include <QPushButton>

class ZoomWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ZoomWidget(QWidget *parent = nullptr);

signals:
    void zoomInRequested();
    void zoomOutRequested();

private:
    QPushButton *zoomInButton;
    QPushButton *zoomOutButton;

    void setupLayout();
};

#endif // ZOOMWIDGET_H
