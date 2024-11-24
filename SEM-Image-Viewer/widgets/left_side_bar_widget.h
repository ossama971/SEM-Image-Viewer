#ifndef LEFT_SIDE_BAR_WIDGET_H
#define LEFT_SIDE_BAR_WIDGET_H

#include "file_browser_widget.h"
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


class LeftSidebarWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LeftSidebarWidget(QWidget *parent = nullptr);

public:
    void setVisible(bool visible) override;

signals:
    void onVisibilityChange(bool visible);

public slots:
    void setMaxMinWidth(int mn, int mx);

private slots:
    void expand();
    void collapse();

private:
    FileBrowserWidget* _fileBrowser;
};

#endif // LEFT_SIDE_BAR_WIDGET_H


