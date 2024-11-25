#ifndef IMAGEINFOBAR_H
#define IMAGEINFOBAR_H

#include <QWidget>
#include <QLabel>

class ImageInfoBar : public QWidget
{
    Q_OBJECT

public:
    explicit ImageInfoBar(QWidget *parent = nullptr);
    void setDimensions(int width, int height);
    void setZoomPercentage(double zoom);
    void setMousePosition(int x, int y);
    void setImageSize(int width, int height); // New method to set image size

private:
    QLabel *dimensionsLabel = nullptr;
    QLabel *zoomLabel = nullptr;
    QLabel *positionLabel = nullptr;
    int imageWidth;  // Store image width
    int imageHeight; // Store image height
};

#endif // IMAGEINFOBAR_H
