#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "../data/session_data.h"
#include <mutex>

class Workspace
{
private:
    Workspace() = default;

public:
    ~Workspace();

    static Workspace* Instance();
    SessionData& getActiveSession();

    static void destroyInstance();

private:
    SessionData _activeSession;

    static Workspace* m_instance;
    static std::recursive_mutex m_mutex;
};

#endif // WORKSPACE_H
