#include "logger_controller.h"

loggerController::loggerController() {
    Logger* _engine = Logger::instance();
    Logger::connect(_engine,&Logger::onCreateLogMessage,this,&loggerController::createLogMessage);
    Logger::connect(_engine,&Logger::onCreateLogMessageWithProgressBar,this,&loggerController::createLogMessageWithProgressBar);
    Logger::connect(_engine,&Logger::onupdateProgressBar,this,&loggerController::updateProgressBar);
}

loggerController &loggerController::instance()
{
    static loggerController instance;
    return instance;
}



void loggerController::setLoggerWidget(LoggerWidget *widget){
    _loggerWidgetPtr=widget;
    if(_loggerWidgetPtr){
        connect(_loggerWidgetPtr,&LoggerWidget::showSelectedType,this,&loggerController::FilterMessagesByType);
    }
}



void loggerController::createLogMessage( IMessage* msg){
    LogCard * _card=new LogCard(msg->GetType(),msg->GetMessage(),msg->GetBody(),false,msg->GetPath());
    LoggerCards[msg->GetId()]=_card;
    _loggerWidgetPtr->addLogCard(_card);
    _messageList.append(msg);

}



void loggerController::createLogMessageWithProgressBar( IMessage* msg,int itemCount){
    LogCard * _card=new LogCard(msg->GetType(),msg->GetMessage(),msg->GetBody(),true,msg->GetPath());
    _card->getProgressBar()->setMaxIterations(itemCount);
    LoggerCards[msg->GetId()]=_card;
    _loggerWidgetPtr->addLogCard(_card);
    _messageList.append(msg);

}
void loggerController::updateProgressBar(int id,int value){
    LoggerCards[id]->getProgressBar()->incrementProgress();
}

loggerController::~loggerController(){
    for(auto it :_messageList){
        if(it !=nullptr){
            delete it;
        }
    }
}

void loggerController::FilterMessagesByType(QString type,QString _text){

    for(auto msg :_messageList )  {
        if(msg->GetType()==type ||type==""){
            if(_text==""||msg->GetMessage().contains(_text)){
             LogCard * _card=new LogCard(msg->GetType(),msg->GetMessage(),msg->GetBody(),false,msg->GetPath());
            _loggerWidgetPtr->addLogCard(_card);
            }
        }
    }

}
