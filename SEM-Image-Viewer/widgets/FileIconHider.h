#ifndef FILEICONHIDER_H
#define FILEICONHIDER_H

#include <QAbstractFileIconProvider>

class FileIconHider : public QAbstractFileIconProvider
{
public:
    FileIconHider();
    ~FileIconHider();

    QIcon icon(IconType type) const override;
    QIcon icon(const QFileInfo &info) const override;
};

#endif // FILEICONHIDER_H
