#ifndef LOGGER_H
#define LOGGER_H

#include "../data/imessage.h"

#include <QObject>
#include <mutex>
#include <atomic>
#include <QMap>
class Logger : public QObject
{
    Q_OBJECT
public:
    enum class MessageTypes
    {
        error,
        warning,
        info
    };

    enum class MessageOption
    {
        with_path,
        without_path,
    };

    enum class MessageID
    {
        filter_applied,
        already_applied,
        images_loading_started,
        images_loading_finished,
        undo_applied,
        redo_applied,
        undo_stack_is_empty,
        redo_stack_is_empty,
        exporting_images,
        batch_filter_applied,
        saving_session,
        loading_session,
        differerence_images,
        no_image_selected,
        error_in_save,
        saved_successfully,
        file_already_exists,
        no_difference,
        operation_in_progress,
        insufficient_permissions,
        exporting_error,
    };

    static QMap<MessageID, QString> LoggerMap;

    ~Logger();

    // Singleton access method
    static Logger *instance();

    void logMessage(Logger::MessageTypes msgtype, Logger::MessageID msgID, Logger::MessageOption msgOptians, QVector<QString> args, QString path = "");
    int logMessageWithProgressBar(Logger::MessageTypes, Logger::MessageID, Logger::MessageOption, QVector<QString> args, int itemCount, QString path = "");
    void updateProgressBar(int id, int value);

signals:
    void onCreateLogMessage(IMessage *msg);
    void onCreateLogMessageWithProgressBar(IMessage *msg, int itemCount);
    void onupdateProgressBar(int id, int value);

private:
    Logger();

    static void destroyInstance();

    int getNextMessageId();

    QString CreateMessage(MessageID msgCode, QVector<QString> &args);

    static Logger *m_instance;
    static std::recursive_mutex m_mutex;
    static std::atomic<int> m_nextMessageId;
    static QList<QWidget *> m_widgets;
    std::atomic<int> m_messageIdCounter{0};
};

#endif // LOGGER_H
