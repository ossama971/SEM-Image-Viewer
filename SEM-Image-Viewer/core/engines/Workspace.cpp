#include "Workspace.h"

std::unique_ptr<Workspace> Workspace::m_instance = nullptr;
std::mutex Workspace::m_mutex;

SessionData& Workspace::getActiveSession() {
    return _activeSession;
}

std::unique_ptr<Workspace>& Workspace::Instance() {
  std::lock_guard<std::mutex> lock(m_mutex);
  if (!m_instance) {
    m_instance = std::unique_ptr<Workspace>(new Workspace());
  }
  return m_instance;
}
