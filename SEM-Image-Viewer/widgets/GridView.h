#ifndef GRIDVIEW_H
#define GRIDVIEW_H

#include <QWidget>
#include <QListView>
#include <QScrollBar>
#include <QCheckBox>
#include <QItemSelectionModel>

#include "../models/ImageDataModel.h"

class GridView : public QWidget {
    Q_OBJECT

signals:
    void openDiffViewRequested(Image* image1, Image* image2);
    void openDiffView();

public:
    explicit GridView(QWidget *parent = nullptr);

private slots:
    void handleSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
    void toggleSelectAll(QCheckBox *checkbox, bool checked);
    void showContextMenu(const QPoint &pos);
    void onImageChanged(Image* newImage);
    void onScroll(int value);
    void openInDiffView();

private:
    QListView *listView;
    ImageDataModel *imageDataModel;

    void setModel(ImageDataModel *model);
    void loadImages(const QString &path);
    void initializeGrid();
    Image* firstImage;
    Image* secondImage;
};

#endif // GRIDVIEW_H
