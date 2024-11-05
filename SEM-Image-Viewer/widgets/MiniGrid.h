#ifndef MINIGRID_H
#define MINIGRID_H

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include "../models/ImageDataModel.h"

class MiniGrid : public QWidget {
    Q_OBJECT

public:
    explicit MiniGrid(QWidget *parent = nullptr);

private:
    QListView *listView;
    ImageDataModel *imageDataModel;
    QPushButton *leftButton;
    QPushButton *rightButton;

    void setModel(ImageDataModel *model);
    void initializeMiniGrid();
    void loadImages(const QString &path);

private slots:
    void scrollLeft();
    void scrollRight();
};

#endif // MINIGRID_H
