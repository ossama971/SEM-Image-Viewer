#include "mainwindow.h"
#include <QApplication>

#include "widgets/LoggerWidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(R"(
QMainWindow {
    background-color: #2B2B2B; /* Dark background color */
    color: #D3D3D3;            /* Light text color */
}

/* Menu bar styling */
QMenuBar {
    background-color: #333333;
    color: #D3D3D3;
}
QMenuBar::item {
    background-color: #333333;
    color: #D3D3D3;
}
QMenuBar::item:selected {
    background-color: #444444;
}

/* Toolbar styling */
QToolBar {
    background-color: #333333;
    border: 1px solid #444444;
}

/* Tool buttons in the toolbar */
QToolButton {
    background-color: #333333;
    color: #D3D3D3;
    border: none;
}
QToolButton:hover {
    background-color: #444444;
}
QToolButton:checked {
    background-color: #555555;
}

/* Styling for QSplitter */
QSplitter {
    background-color: #2C2C2C;
}
QSplitter::handle {
    background-color: #1E1E1E; /* Color for the splitter handle */
}

/* Styling for QTreeView and QListView */
QTreeView, QListView {
    background-color: #333333;
    color: #D3D3D3;
    selection-background-color: #555555;
    selection-color: #FFFFFF;
    border: 1px solid #444444;
}

/* Styling for QTableView */
QTableView {
    background-color: #333333;
    color: #D3D3D3;
    gridline-color: #444444;
    selection-background-color: #555555;
}

/* Text input fields (QLineEdit, QComboBox) */
QLineEdit, QComboBox {
    background-color: #3A3A3A;
    color: #D3D3D3;
    border: 1px solid #444444;
    padding: 4px;
}

/* Styling for QPushButton */
QPushButton {
    color: #D3D3D3;
    padding: 4px;
    border: none;
    background-color: transparent;
}
QPushButton:hover {
    background-color: #444444;
}
QPushButton:pressed {
    background-color: #555555;
}

/* Styling for QLabel (used for displaying image placeholders, etc.) */
QLabel {
    color: #D3D3D3;
    background-color: #2B2B2B;
}

/* Tabs styling */
QTabWidget::pane {
    border: 1px solid #444444;
}
QTabBar::tab {
    background-color: #333333;
    color: #D3D3D3;
    padding: 8px;
    margin: 2px;
}
QTabBar::tab:selected {
    background-color: #444444;
}

/* Styling for QStatusBar */
QStatusBar {
    background-color: #333333;
    color: #D3D3D3;
}

/* Styling for QListWidget items */
QListWidget::item {
    background-color: #333333;
    color: #D3D3D3;
    border: 1px solid #444444;
    padding: 4px;
}
QListWidget::item:selected {
    background-color: #555555;
}
  QMenu {
        background-color: #2b2b2b;
        color: #d3d3d3;
        border: 1px solid #3e3e3e;
    }

    QMenu::item {
        background-color: transparent;
        color: #d3d3d3;
        padding: 5px 20px;
        margin: 2px;
    }

    QMenu::item:selected {
        background-color: #3e3e3e;
    }

    QMenu::separator {
        height: 1px;
        background: #444444;
        margin: 5px 0;
    }
 QAction {
        color: #d3d3d3;
    }

)");
    MainWindow w;
    w.show();
    return a.exec();

}
