#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "widgets/bottom_middle_widget.h"
#include "widgets/left_side_bar_widget.h"
#include "widgets/right_side_bar_widget.h"
#include "widgets/top_middle_widget.h"
#include "widgets/menu_bar_widget.h"
#include "widgets/exit_dialog_widget.h"
#include "widgets/mini_grid_widget.h"
#include "widgets/toolbar_widget.h"

#include <QMainWindow>
#include <QWidget>
#include <QCloseEvent>
#include <QSplitter>

#include "widgets/image_widget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    // bool saveSession(MainWindow *parent)
    ~MainWindow();
public slots:
    void onShowLeftSidebarClicked(bool isChecked);
    void onShowRightSidebarClicked(bool isChecked);
    void onShowLoggerClicked(bool isChecked);
    void onShowImageClicked(bool isChecked);
    void showMiniGridClicked(bool isChecked);
    void onSaveChangesClicked();
    void onCloseAllClicked();

private slots:
    void applyTheme();
protected:
    void closeEvent(QCloseEvent *event) override;

private:
    Ui::MainWindow *ui = nullptr;
    ImageWidget *imageWidget = nullptr;
    LeftSidebarWidget *leftSidebarWidget = nullptr;
    RightSidebarWidget *rightSidebarWidget = nullptr;
    TopMiddleWidget *topMiddleWidget = nullptr;
    BottomMiddleWidget *bottomMiddleWidget = nullptr;
    QSplitter *middleSplitter = nullptr;
    QSplitter *mainSplitter = nullptr;
    MenuBarWidget *menuBarWidget = nullptr;
    MiniGrid *miniGrid = nullptr;
    ToolbarWidget *toolbarWidget = nullptr;

    QProgressBar *progressBar = nullptr;
};


#endif // MAINWINDOW_H
