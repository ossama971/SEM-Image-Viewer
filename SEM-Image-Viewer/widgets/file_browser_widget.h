#ifndef FILE_BROWSER_WIDGET_H
#define FILE_BROWSER_WIDGET_H

#include "file_icon_hider.h"
#include "image_filter_proxy_model.h"
#include "../core/data/image_repository.h"
#include <QWidget>
#include <QPushButton>
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
    void setRoot(QString path);
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
    ImageRepository* _imageRepo;

    QTreeView *treeView;
    QFileSystemModel *model;
    FileIconHider *iconProvider;
    ImageFilterProxyModel *proxyModel;
    QLineEdit *searchBox;
    class ImageDialog* imageDialog;
};

#endif // FILE_BROWSER_WIDGET_H
