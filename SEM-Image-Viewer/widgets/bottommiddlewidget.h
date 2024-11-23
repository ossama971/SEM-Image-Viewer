#ifndef BOTTOMMIDDLEWIDGET_H
#define BOTTOMMIDDLEWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSplitter>
#include <QApplication>
#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QGuiApplication>
#include <QScreen>



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

#endif // BOTTOMMIDDLEWIDGET_H


