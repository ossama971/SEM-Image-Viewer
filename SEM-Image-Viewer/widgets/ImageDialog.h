#ifndef IMAGEDIALOG_H
#define IMAGEDIALOG_H

#include "../core/data/ImageRepository.h"
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

#endif // IMAGEDIALOG_H
