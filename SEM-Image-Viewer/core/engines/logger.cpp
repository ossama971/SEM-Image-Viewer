#include "logger.h"

Logger *Logger::m_instance = nullptr;
std::recursive_mutex Logger::m_mutex;

QMap<Logger::MessageID, QString> Logger::LoggerMap;
Logger *Logger::instance()
{
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  if (!m_instance)
  {
    m_instance = new Logger();

    LoggerMap[MessageID::filter_applied] = " %1 : Filter applied successfully";
    LoggerMap[MessageID::images_loading_started] = "Folder at path %1 loaded successfully";
    LoggerMap[MessageID::images_loading_finished] = "Started loading folder %1";
    LoggerMap[MessageID::undo_applied] = "Undo filter %1 successfully";
    LoggerMap[MessageID::redo_applied] = "Redo filter %1 successfully";
    LoggerMap[MessageID::undo_stack_is_empty] = "Redo Stack For Image %1 is Empty";
    LoggerMap[MessageID::redo_stack_is_empty] = "Undo Stack For Image %1 is Empty";
    LoggerMap[MessageID::exporting_images] = "Exporting images to %1";
    LoggerMap[MessageID::batch_filter_applied] = "Batch Filter applied";
    LoggerMap[MessageID::saving_session] = "Saving session to %1 file";
    LoggerMap[MessageID::loading_session] = "Loading session from %1 file";
    LoggerMap[MessageID::differerence_images] = "Difference Images feature has been applied successfully";
    LoggerMap[MessageID::already_applied] = "Filter %1 already applied";
    LoggerMap[MessageID::no_image_selected] = "No image selected";
    LoggerMap[MessageID::error_in_save] = "Error in saving session";
    LoggerMap[MessageID::saved_successfully] = "Session saved successfully";
    LoggerMap[MessageID::file_already_exists] = "Json file %1 already exists in the directory";
    LoggerMap[MessageID::no_difference] = "No difference between two images";
    LoggerMap[MessageID::operation_in_progress] = "Can't load images while having batch operation in progress";
    std::atexit(destroyInstance);
  }
  return m_instance;
}

void Logger::destroyInstance()
{
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  if (m_instance)
  {
    delete m_instance;
    m_instance = nullptr;
  }
}

Logger::Logger() = default;

Logger::~Logger()
{
  // Destructor to prevent accidental deletion
}

int Logger::getNextMessageId()
{
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  return m_messageIdCounter++;
}

void Logger::logMessage(Logger::MessageTypes msgtype, Logger::MessageID msgID,
                        Logger::MessageOption msgOptians, QVector<QString> args,
                        QString path)
{
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  int _id = getNextMessageId();
  QString _msgHeader = CreateMessage(msgID, args);

  IMessage *_msg = new IMessage(_id, static_cast<int>(msgtype), _msgHeader);

  if (msgOptians == MessageOption::with_path)
  {
    _msg->SetPath(path);
  }
  emit onCreateLogMessage(_msg);
}

int Logger::logMessageWithProgressBar(Logger::MessageTypes msgtype,
                                      Logger::MessageID msgID,
                                      Logger::MessageOption msgOptians,
                                      QVector<QString> args, int itemCount,
                                      QString path)
{
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  int _id = getNextMessageId();
  QString _msgHeader = CreateMessage(msgID, args);

  IMessage *_msg = new IMessage(_id, static_cast<int>(msgtype), _msgHeader);

  if (msgOptians == MessageOption::with_path)
  {
    _msg->SetPath(path);
  }
  emit onCreateLogMessageWithProgressBar(_msg, itemCount);
  return _id;
}

void Logger::updateProgressBar(int id, int value)
{
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  emit onupdateProgressBar(id, value);
}

QString Logger::CreateMessage(MessageID msgCode, QVector<QString> &args)
{
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  QString res = LoggerMap[msgCode];

  for (auto it : args)
  {
    res = res.arg(it);
  }

  return res;
}
