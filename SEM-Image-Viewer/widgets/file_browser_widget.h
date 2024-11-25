#ifndef FILE_BROWSER_WIDGET_H
#define FILE_BROWSER_WIDGET_H

#include "file_icon_hider.h"
#include "image_filter_proxy_model.h"
#include "../core/data/image_repository.h"
#include <QWidget>
#include <QTreeView>
#include <QDir>
#include <QFileSystemModel>
#include <QLineEdit>

class FileBrowserWidget : public QWidget
{
    Q_OBJECT
public:
    explicit FileBrowserWidget(QWidget *parent = nullptr);
    ~FileBrowserWidget();

private:
    void Initialize();

public:
    void setRoot(const QString &path);
    void setImageFilter(std::filesystem::path path);

private slots:
    void onOpenFolder();
    void onOpenFile();
    void onCollapse();
    void onSearch();
    void onTreeClick(const QModelIndex &index);
    void onDirectoryChanged(const std::string newDir, std::vector<Image*> newImages, bool image_load);

signals:
    void expand();
    void collapse();

private:
    QDir _rootDir;
    ImageRepository* _imageRepo = nullptr;
    QTreeView *treeView = nullptr;
    QFileSystemModel *model = nullptr;
    FileIconHider *iconProvider = nullptr;
    ImageFilterProxyModel *proxyModel = nullptr;
    QLineEdit *searchBox = nullptr;
    class ImageDialog* imageDialog = nullptr;
};

#endif // FILE_BROWSER_WIDGET_H
