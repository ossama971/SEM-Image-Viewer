#ifndef FILEBROWSERWIDGET_H
#define FILEBROWSERWIDGET_H

#include "FileIconHider.h"
#include "ImageFilterProxyModel.h"
#include "../core/data/ImageRepository.h"
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

#endif // FILEBROWSERWIDGET_H
