#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "../data/SessionData.h"

class Workspace
{
private:
    Workspace() = default;
public:
    ~Workspace() = default;

    SessionData& getActiveSession();
    static Workspace& Instance();
	
private:
    SessionData _activeSession;

    static Workspace _instance;
};

#endif // WORKSPACE_H
