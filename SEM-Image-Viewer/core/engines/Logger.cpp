#include "Logger.h"

Logger *Logger::m_instance = nullptr;
std::recursive_mutex Logger::m_mutex;

QMap<Logger::MessageID, QString> Logger::LoggerMap;
Logger *Logger::instance() {
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  if (!m_instance) {
    m_instance = new Logger();

    LoggerMap[MessageID::FILTER_APPLIED] = " %1 : Filter applied successfully";
    LoggerMap[MessageID::IMAGES_LOADING_FINISHED] = "Folder at path %1 loaded successfully";
    LoggerMap[MessageID::IMAGES_LOADING_STARTED] = "Started loading folder %1";
    LoggerMap[MessageID::UNDO_APPLIED] = "Undo filter %1 successfully";
    LoggerMap[MessageID::REDO_APPLIED] = "Redo filter %1 successfully";
    LoggerMap[MessageID::UNDO_STACK_IS_EMPTY] = "Redo Stack For Image %1 is Empty";
    LoggerMap[MessageID::REDO_STACK_IS_EMPTY] = "Undo Stack For Image %1 is Empty";
    LoggerMap[MessageID::EXPORTING_IMAGES] = "Exporting images to %1";

    std::atexit(destroyInstance);
  }
  return m_instance;
}

void Logger::destroyInstance() {
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  if (m_instance) {
    delete m_instance;
    m_instance = nullptr;
  }
}

Logger::Logger() = default;

Logger::~Logger() {
  // Destructor to prevent accidental deletion
}

int Logger::getNextMessageId() {
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  return m_messageIdCounter++;
}

void Logger::logMessage(Logger::MessageTypes msgtype, Logger::MessageID msgID,
                        Logger::MessageOptian msgOptians, QVector<QString> args,
                        QString details, QString path) {
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  int _id = getNextMessageId();
  QString _msgHeader = CreateMessage(msgID, args);

  IMessage *_msg = new IMessage(_id, static_cast<int>(msgtype), _msgHeader);

  if (msgOptians == MessageOptian::WITH_DETAILS_AND_PATH) {
    _msg->SetPath(path);
    _msg->SetBody(details);
  } else if (msgOptians == MessageOptian::WITH_DETAILS) {
    _msg->SetBody(details);
  }
  emit onCreateLogMessage(_msg);
}

int Logger::logMessageWithProgressBar(Logger::MessageTypes msgtype,
                                      Logger::MessageID msgID,
                                      Logger::MessageOptian msgOptians,
                                      QVector<QString> args, int itemCount,
                                      QString details, QString path) {
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  int _id = getNextMessageId();
  QString _msgHeader = CreateMessage(msgID, args);

  IMessage *_msg = new IMessage(_id, static_cast<int>(msgtype), _msgHeader);

  if (msgOptians == MessageOptian::WITH_DETAILS_AND_PATH) {
    _msg->SetPath(path);
    _msg->SetBody(details);
  } else if (msgOptians == MessageOptian::WITH_DETAILS) {
    _msg->SetBody(details);
  }
  emit onCreateLogMessageWithProgressBar(_msg, itemCount);
  return _id;
}

void Logger::updateProgressBar(int id, int value) {
  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  emit onupdateProgressBar(id, value);
}

QString Logger::CreateMessage(MessageID msgCode, QVector<QString> &args) {

  std::scoped_lock<std::recursive_mutex> lock(m_mutex);
  QString res = LoggerMap[msgCode];

  for (auto it : args) {
    res = res.arg(it);
  }

  return res;
}
