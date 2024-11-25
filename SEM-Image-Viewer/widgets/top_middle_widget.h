#ifndef TOP_MIDDLE_WIDGET_H
#define TOP_MIDDLE_WIDGET_H

#include <QWidget>
#include <QVBoxLayout>

#include "image_widget.h"
#include "grid_view_widget.h"
#include "diff_view_widget.h"
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
    void onimageViewButtonClicked();
    void ondiffViewButtonClicked();
    void ongridViewButtonClicked();

private:
    ToolbarWidget *toolbar = nullptr;
    ImageWidget *image = nullptr;
    GridView *gridView = nullptr;
    DiffViewWidget *diffView = nullptr;
    QVBoxLayout *topMiddleLayout = nullptr;

signals:
    void selectDiffView();
};

#endif // TOP_MIDDLE_WIDGET_H
