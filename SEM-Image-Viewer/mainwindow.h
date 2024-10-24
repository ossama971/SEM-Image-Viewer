#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "widgets/image_widget.h"
#include "modules/image_processor.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_imporbtn_clicked();  // Slot to handle the import button click

private:
    Ui::MainWindow *ui;
    ImageProcessor *imageProcessor;
    ImageWidget *imageWidget;  // Custom widget containing only the image viewer
};

#endif // MAINWINDOW_H
