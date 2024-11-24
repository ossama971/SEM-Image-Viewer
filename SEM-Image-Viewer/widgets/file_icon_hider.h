#ifndef FILE_ICON_HIDER_H
#define FILE_ICON_HIDER_H

#include <QAbstractFileIconProvider>

class FileIconHider : public QAbstractFileIconProvider
{
public:
    FileIconHider();
    ~FileIconHider();

    QIcon icon(IconType type) const override;
    QIcon icon(const QFileInfo &info) const override;
};

#endif // FILE_ICON_HIDER_H
