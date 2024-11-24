#ifndef IMAGE_DIALOG_H
#define IMAGE_DIALOG_H

#include "../core/data/image_repository.h"
#include <QWidget>
#include <QString>

class ImageDialog : public QWidget
{
    Q_OBJECT
public:
    ImageDialog(QWidget* parent = nullptr);

    void openFolder(ImageRepository* imageRepo, QWidget* parent = nullptr);
    void openFile(ImageRepository* imageRepo, QWidget* parent = nullptr);
};

#endif // IMAGE_DIALOG_H
