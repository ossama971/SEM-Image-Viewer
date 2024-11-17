#ifndef TOPMIDDLEWIDGET_H
#define TOPMIDDLEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGuiApplication>
#include <QScreen>
#include <QToolButton>

#include "image_widget.h"
#include "GridView.h"
#include "DiffViewWidget.h"
#include "toolbar_widget.h"

class TopMiddleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TopMiddleWidget(QWidget *parent = nullptr);

public:
    void setVisible(bool visible) override;

signals:
    void onVisibilityChange(bool visible);

public slots:
    void setMaxMinHeight(int mn, int mx);
    void openDiffView();

private slots:
    void onimageViewButtonClicked();
    void ondiffViewButtonClicked();
    void ongridViewButtonClicked();

private:
    ToolbarWidget *toolbar;
    ImageWidget *image;
    GridView *gridView;
    DiffViewWidget *diffView;
    QVBoxLayout *topMiddleLayout;

signals:
    void selectDiffView();
};

#endif // TOPMIDDLEWIDGET_H
