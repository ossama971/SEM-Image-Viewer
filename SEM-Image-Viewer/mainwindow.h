#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QSplitter>
#include "widgets/bottommiddlewidget.h"
#include "widgets/leftsidebarwidget.h"
#include "widgets/rightsidebarwidget.h"
#include "widgets/topmiddlewidget.h"
#include "widgets/menubarwidget.h"
#include "widgets/savedialogwidget.h"
#include "widgets/MiniGrid.h"
#include "widgets/toolbar_widget.h"
// #include "core/engines/JsonVisitor.h"

#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QIcon>
#include <QCloseEvent>
#include <QMessageBox>

#include "widgets/image_widget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void onShowLeftSidebarClicked(bool isChecked);
    void onShowRightSidebarClicked(bool isChecked);
    void onShowLoggerClicked(bool isChecked);
    void onShowImageClicked(bool isChecked);
    void showMiniGridClicked(bool isChecked);
    void onSaveChangesClicked();

private slots:
    void applyTheme();
protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui;
    ImageWidget *imageWidget;

    LeftSidebarWidget *leftSidebarWidget;
    RightSidebarWidget *rightSidebarWidget;
    TopMiddleWidget *topMiddleWidget;
    BottomMiddleWidget *bottomMiddleWidget;
    QSplitter *middleSplitter;
    QSplitter *mainSplitter;
    MenuBarWidget *menuBarWidget;
    MiniGrid *miniGrid;
    ToolbarWidget *toolbarWidget;

    QThread *saveThread = nullptr;
};


#endif // MAINWINDOW_H
