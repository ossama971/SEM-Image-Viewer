#ifndef LOGGER_CONTROLLER_H
#define LOGGER_CONTROLLER_H

#include <QObject>
#include <QMap>
#include "logger_widget.h"
#include "log_card_widget.h"
#include "../core/engines/logger.h"

class loggerController :public QObject
{
    Q_OBJECT
private:

    QMap<int ,LogCard*> LoggerCards;
    QList<IMessage* > _messageList;
    loggerController();
    loggerController(const loggerController &) = delete;
    loggerController &operator=(const loggerController &) = delete;
    LoggerWidget *_loggerWidgetPtr = nullptr;
    bool logCardColor;

public:
    static loggerController &instance();
    void setLoggerWidget(LoggerWidget *widget);
    ~loggerController();

private:
    int countLogs(const QString &type);

public slots:

    void FilterMessagesByType(const QString &type, const QString &_text);
    void createLogMessage( IMessage* msg);
    void createLogMessageWithProgressBar( IMessage* msg,int itemCount);
    void updateProgressBar(int id,int value);

    void updateLogStats();
};

#endif
