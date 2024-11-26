#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication a(argc, argv);
    QFile file(":/styles/light-mode.qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString styleSheet = file.readAll();
        a.setStyleSheet(styleSheet);
        file.close();
    } else {
        // qDebug() << "Failed to load stylesheet";
    }
    MainWindow w;
    w.setWindowIcon(QIcon(":/icons/2383605.svg"));
    w.show();
    return a.exec();

}
