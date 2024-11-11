#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "../data/SessionData.h"

#include <memory>
#include <mutex>

class Workspace
{
private:
    Workspace() = default;

public:
    ~Workspace() = default;

    SessionData& getActiveSession();
    static std::unique_ptr<Workspace>& Instance();
	
private:
    SessionData _activeSession;

    static std::unique_ptr<Workspace> m_instance;
    static std::mutex m_mutex;
};

#endif // WORKSPACE_H
