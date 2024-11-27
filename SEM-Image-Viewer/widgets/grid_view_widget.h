#ifndef GRID_VIEW_WIDGET_H
#define GRID_VIEW_WIDGET_H

#include <QWidget>
#include <QListView>
#include <QCheckBox>
#include <QItemSelectionModel>

class Image;

class GridView : public QWidget {
    Q_OBJECT

signals:
    void openDiffViewRequested(Image* image1, Image* image2);
    void openDiffView();

public:
    explicit GridView(QWidget *parent = nullptr, class ImageDataModel *dataModel = nullptr);
    std::vector<int> getSelectedImages();

private slots:
    void handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void toggleSelectAll(QCheckBox *checkbox, bool checked);
    void showContextMenu(const QPoint &pos);
    void onImageChanged(Image* newImage);
    void onScroll(int value);
    void openInDiffView();
    void preImageStateUpdate();
    void postImageStateUpdate();

private:
    QListView *listView = nullptr;
    ImageDataModel *imageDataModel = nullptr;
    Image* firstImage = nullptr;
    Image* secondImage = nullptr;
    QModelIndex _selectedIndex;

    void setModel(ImageDataModel *model);
    void loadImages(const QString &path);
    void initializeGrid();
};

#endif // GRID_VIEW_WIDGET_H
