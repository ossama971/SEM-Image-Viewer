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
        Logger::connect(_loggerWidgetPtr,&LoggerWidget::getLogStats,this,&loggerController::updateLogStats);
    }
}

void loggerController::createLogMessage( IMessage* msg){
    LogCard * _card=new LogCard(msg->GetType(),msg->GetMessage(),false,msg->GetId()%2,msg->GetPath());
    LoggerCards[msg->GetId()]=_card;
    _loggerWidgetPtr->addLogCard(_card);
    _messageList.append(msg);

    updateLogStats();
}

void loggerController::createLogMessageWithProgressBar( IMessage* msg,int itemCount){
    LogCard * _card=new LogCard(msg->GetType(),msg->GetMessage(),true,msg->GetId()%2,msg->GetPath());
    _card->getProgressBar()->setMaxIterations(itemCount);
    LoggerCards[msg->GetId()]=_card;
    _loggerWidgetPtr->addLogCard(_card);
    _messageList.append(msg);

    updateLogStats();
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

int loggerController::countLogs(const QString &type) {
    int count = 0;

    for (auto msg : _messageList) {

        if (type.isEmpty() || msg->GetType() == type) {
            count++;
        }
    }

    return count;
}

void loggerController::FilterMessagesByType(const QString &type, const QString &_text) {
    for (auto msg : _messageList) {

        if (type.isEmpty() || msg->GetType() == type) {

            if (_text.isEmpty() || msg->GetMessage().contains(_text)) {

                LogCard *_card = new LogCard(
                    msg->GetType(),
                    msg->GetMessage(),
                    false,
                    msg->GetId() % 2,
                    msg->GetPath()
                    );
                _loggerWidgetPtr->addLogCard(_card);
            }
        }
    }
}

void loggerController::updateLogStats() {
    _loggerWidgetPtr->updateLogs(countLogs("Info"), countLogs("Warning"), countLogs("Error"));
}
