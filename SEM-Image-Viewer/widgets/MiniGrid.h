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

private slots:
    void scrollLeft();
    void scrollRight();
    void handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void onImageChanged(Image* newImage);
};

#endif // MINIGRID_H
