#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include "../core/data/ImageRepository.h"
#include <QString>

class ImageDialog
{
public:
    static QString openFolder(ImageRepository* imageRepo, QWidget* parent = nullptr);
    static QString openFile(ImageRepository* imageRepo, QWidget* parent = nullptr);
};

#endif // IMAGEDIALOG_H
