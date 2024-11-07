#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "../engines/ImageSession.h"

class Workspace
{
private:
    Workspace() = default;
public:
    ~Workspace() = default;

    ImageSession& getActiveSession();
    static Workspace& Instance();
	
private:
    ImageSession _activeSession;

    static Workspace _instance;
};

#endif // WORKSPACE_H
