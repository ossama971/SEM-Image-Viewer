#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QWidget>
#include <QListView>
#include "../models/ImageDataModel.h"

class GridView : public QWidget {
    Q_OBJECT

public:
    explicit GridView(QWidget *parent = nullptr);

private:
    QListView *listView;
    ImageDataModel *imageDataModel;

    void setModel(ImageDataModel *model);
    void initializeGrid();
    void loadImages(const QString &path);
};

#endif // GRIDVIEW_H
