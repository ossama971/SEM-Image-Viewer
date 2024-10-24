#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);
    void setImage(const QPixmap &pixmap);  // Public method to set the image on the label

private:
    QLabel *imageLabel;  // Label to display the image
};

#endif // IMAGEWIDGET_H
