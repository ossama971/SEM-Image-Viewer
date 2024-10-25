#ifndef IMAGEWIDGET_H
#define IMAGEWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <optional>

class ImageWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ImageWidget(QWidget *parent = nullptr);
    void loadAndDisplayImage(const QString &imagePath);

private:
    QLabel *imageLabel;
    std::optional<QPixmap> loadAndPrepareImage(const QString &path, const QSize &targetSize);
    void setImage(const QPixmap &pixmap);

signals:
    void imageLoadFailed();  // Signal to notify MainWindow of a failure
};

#endif // IMAGEWIDGET_H
