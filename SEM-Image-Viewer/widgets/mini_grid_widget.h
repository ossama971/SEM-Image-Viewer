#ifndef MINI_GRID_WIDGET_H
#define MINI_GRID_WIDGET_H

#include <QWidget>
#include <QListView>
#include <QPushButton>

class Image;

class MiniGrid : public QWidget {
    Q_OBJECT

public:
    explicit MiniGrid(QWidget *parent = nullptr, class ImageDataModel *dataModel = nullptr);

private:
    QListView *listView = nullptr;
    ImageDataModel *imageDataModel = nullptr;
    QPushButton *leftButton = nullptr;
    QPushButton *rightButton = nullptr;

    void setModel(ImageDataModel *model);
    void initializeMiniGrid();

private slots:
    void scrollLeft();
    void scrollRight();
    void handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onImageChanged(Image* newImage);
    void preImageStateUpdate();
    void postImageStateUpdate();

private:
    QModelIndex _selectedIndex;
};

#endif // MINI_GRID_WIDGET_H
