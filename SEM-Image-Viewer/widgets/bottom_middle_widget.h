#ifndef BOTTOM_MIDDLE_WIDGET_H
#define BOTTOM_MIDDLE_WIDGET_H

#include <QWidget>

class BottomMiddleWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BottomMiddleWidget(QWidget *parent = nullptr);

public:
    void setVisible(bool visible) override;

signals:
    void onVisibilityChange(bool visible);

public slots:
    void setMaxMinHeight(int mn, int mx);
    void adjustSizeBasedOnLayout(bool isExpanded);
};

#endif // BOTTOM_MIDDLE_WIDGET_H


