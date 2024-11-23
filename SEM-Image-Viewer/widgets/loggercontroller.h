#ifndef LOGGERCONTROLLER_H
#define LOGGERCONTROLLER_H

#include <QObject>
#include <QMap>
#include "LoggerWidget.h"
#include "logCardWidget.h"
#include "../core/engines/Logger.h"
class loggerController :public QObject
{
    Q_OBJECT
private:

    QMap<int ,LogCard*> LoggerCards;
    QList<IMessage* > _messageList;
    loggerController();
    loggerController(const loggerController &) = delete;
    loggerController &operator=(const loggerController &) = delete;


    LoggerWidget *_loggerWidgetPtr;

public:
    static loggerController &instance();


    void setLoggerWidget(LoggerWidget *widget);
    ~loggerController();
public slots:

    void FilterMessagesByType(QString type, QString _text);
    void createLogMessage( IMessage* msg);
    void createLogMessageWithProgressBar( IMessage* msg,int itemCount);
    void updateProgressBar(int id,int value);

};

#endif
