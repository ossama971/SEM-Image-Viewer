#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QWidget>
#include <QListView>
#include "../models/ImageDataModel.h"
#include <QItemSelectionModel>
#include <QScrollBar>

class GridView : public QWidget {
    Q_OBJECT

public:
    explicit GridView(QWidget *parent = nullptr);

private slots:
    void handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onImageChanged(Image* newImage);
    void onScroll(int value); // New slot to handle scroll events

private:
    QListView *listView;
    ImageDataModel *imageDataModel;

    void setModel(ImageDataModel *model);
    void initializeGrid();
    void loadImages(const QString &path);
};

#endif // GRIDVIEW_H
