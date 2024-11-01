//#include "mainwindow.h"

#include <QApplication>
#include "widgets/LoggerWidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoggerWidget loggerWidget;
    loggerWidget.resize(600, 400);
    loggerWidget.show();
    loggerWidget.addLogMessage("Info", "Initiated Denoising");
    loggerWidget.addLogMessage("Error", "Please enter a valid number");
    loggerWidget.addLogMessage("Error", "Operation didn't complete successfully");
    return a.exec();
}
