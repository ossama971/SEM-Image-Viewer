#ifndef MINIGRID_H
#define MINIGRID_H

#include <QWidget>
#include <QListView>
#include "../models/ImageDataModel.h"

class MiniGrid : public QWidget {
    Q_OBJECT

public:
    explicit MiniGrid(QWidget *parent = nullptr);

private:
    QListView *listView;
    ImageDataModel *imageDataModel;

    void setModel(ImageDataModel *model);
    void initializeMiniGrid();
    void loadImages(const QString &path);
};

#endif // MINIGRID_H
