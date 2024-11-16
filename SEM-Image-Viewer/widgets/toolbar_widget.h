#ifndef TOOLBARWIDGET_H
#define TOOLBARWIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QHBoxLayout>
#include <QButtonGroup>

class ToolbarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ToolbarWidget(QWidget *parent = nullptr);
    QToolButton *button1;
    QToolButton *button2;
    QToolButton *button3;
};

#endif // TOOLBARWIDGET_H
