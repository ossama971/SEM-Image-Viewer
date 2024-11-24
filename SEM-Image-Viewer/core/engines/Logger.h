#ifndef LOGGER_H
#define LOGGER_H

#include "../data/IMessage.h"

#include <QObject>
#include <mutex>
#include <atomic>
#include <QMap>
class Logger : public QObject
{
    Q_OBJECT
public:

    enum class MessageTypes {
        ERROR,
        WRANING,
        INFO
    };
    enum class MessageOptian{
        WITH_DETAILS_AND_PATH,
        WITH_DETAILS,
        WITHOUT_DETIALS
    };
    enum class MessageID{
        FILTER_APPLIED,
        IMAGES_LOADING_STARTED,
        IMAGES_LOADING_FINISHED,
        UNDO_APPLIED,
        REDO_APPLIED,
        UNDO_STACK_IS_EMPTY,
        REDO_STACK_IS_EMPTY,
        EXPORTING_IMAGES,
        BATCH_FILTER_APPLIED,
        SAVING_SESSION,
        LOADING_SESSION,
    };

    static QMap<MessageID,QString> LoggerMap;

    ~Logger();

    // Singleton access method
    static Logger* instance();

    void logMessage(Logger::MessageTypes msgtype,Logger::MessageID msgID,Logger::MessageOptian msgOptians,QVector<QString> args,QString details="",QString path="");
    int logMessageWithProgressBar(Logger::MessageTypes,Logger::MessageID,Logger::MessageOptian,QVector<QString> args,int itemCount,QString details="",QString path="");
    void updateProgressBar(int id, int value);

signals:
    void onCreateLogMessage( IMessage* msg);
    void onCreateLogMessageWithProgressBar( IMessage* msg,int itemCount);
    void onupdateProgressBar(int id, int value);

private:
    Logger();

    static void destroyInstance();


    int getNextMessageId();

    QString CreateMessage(MessageID msgCode,QVector<QString> &args);

    static Logger* m_instance;
    static std::recursive_mutex m_mutex;
    static std::atomic<int> m_nextMessageId;
    static QList<QWidget*> m_widgets;
    std::atomic<int> m_messageIdCounter{0};
};

#endif // LOGGER_H
