#include "Logger.h"

std::unique_ptr<Logger> Logger::m_instance = nullptr;
std::mutex Logger::m_mutex;

std::unique_ptr<Logger> &Logger::instance() {
  std::scoped_lock<std::mutex> lock(m_mutex);
  if (!m_instance) {
    m_instance = std::unique_ptr<Logger>(new Logger());
  }
  return m_instance;
}

void Logger::setModel(std::shared_ptr<MessageDataModel> model) {
  std::scoped_lock<std::mutex> lock(m_mutex);
  m_dataModel = model;
}

void Logger::log(std::unique_ptr<IMessage> msg) {
  std::scoped_lock<std::mutex> lock(m_mutex);
  if (m_dataModel == nullptr) {
    throw std::runtime_error("Logger model is not set");
  }
  m_dataModel->addLogEntry(std::move(msg));
}
