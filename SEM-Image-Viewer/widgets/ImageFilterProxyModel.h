#ifndef IMAGEFILTERPROXYMODEL_H
#define IMAGEFILTERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <qdir.h>

class ImageFilterProxyModel : public QSortFilterProxyModel
{
private:
    QDir _rootDir;
    QString _imageFilter;
    QString _searchText;

public:
    ImageFilterProxyModel(QObject *parent = nullptr);

    void setRootDir(QDir root_dir);
    void setImageFilter(QString image_path);
    void setSearchText(QString search_text);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
};

#endif // IMAGEFILTERPROXYMODEL_H
