#include "workspace.h"
#include <QDebug>
#include <mutex>

Workspace* Workspace::m_instance = nullptr;
std::recursive_mutex Workspace::m_mutex;

Workspace* Workspace::Instance() {
    std::scoped_lock lock(m_mutex);
    if (!m_instance) {
        m_instance = new Workspace();
        std::atexit(destroyInstance);
    }
    return m_instance;
}

void Workspace::destroyInstance() {
    std::scoped_lock lock(m_mutex);
    if (m_instance) {
        delete m_instance;
        m_instance = nullptr;
    }
}

SessionData& Workspace::getActiveSession() {
    std::scoped_lock lock(m_mutex);
    return _activeSession;
}

Workspace::~Workspace() {
}

