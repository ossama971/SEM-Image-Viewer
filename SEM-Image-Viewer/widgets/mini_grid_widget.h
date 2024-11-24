#ifndef MINI_GRID_WIDGET_H
#define MINI_GRID_WIDGET_H

#include <QWidget>
#include <QListView>
#include <QPushButton>
#include "../models/image_data_model.h"

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

#endif // MINI_GRID_WIDGET_H
