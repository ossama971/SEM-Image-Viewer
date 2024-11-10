#include "rightsidebarwidget.h"
#include "sharpen_filter_widget.h"
#include "controllerWidget.h"
#include "edge_extraction_wigdet.h"
#include "historywidget.h"
#include "progressbar.h"
RightSidebarWidget::RightSidebarWidget(QWidget *parent) : QWidget(parent) {
    int mainScreenWidth = QGuiApplication::primaryScreen()->geometry().width();
    int mainScreenHeight = QGuiApplication::primaryScreen()->geometry().height();
    //setStyleSheet("background-color: #000000;");
    setMinimumWidth(mainScreenWidth*0.05);
    setMaximumWidth(mainScreenWidth*0.3);
    Controller &controller = Controller::instance();
    QVBoxLayout *rightSidebarLayout = new QVBoxLayout();


    QWidget *rightContent = new QWidget(parent);
   // rightContent->setStyleSheet("background-color: #627e7c;");
    ContourWidget* _contourWidget=new ContourWidget();
    EdgeExtractionWidget* _edgeExtractionWidget= new EdgeExtractionWidget();
    HistoryWidget * _historyWidget=new HistoryWidget();
    ProgressBarComponent* _progressBar = new ProgressBarComponent();
    rightSidebarLayout->addWidget(_contourWidget);
    rightSidebarLayout->addWidget(_edgeExtractionWidget);
    rightSidebarLayout->addWidget(_progressBar);
    rightSidebarLayout->addWidget(_historyWidget);



    controller.setEdgeExtractionWidget(_edgeExtractionWidget);
    controller.setContourWidget(_contourWidget);
    controller.setHistoryWidget(_historyWidget);


    this->setLayout(rightSidebarLayout);
}



void RightSidebarWidget::setMaxMinWidth(int mn, int mx){
    setMinimumWidth(mn);
    setMaximumWidth(mx);
}
