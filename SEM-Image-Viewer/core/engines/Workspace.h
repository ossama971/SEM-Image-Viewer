#ifndef WORKSPACE_H
#define WORKSPACE_H

#include "../engines/ImageSession.h"

class Workspace
{
private:
    Workspace() = default;
public:
    ~Workspace() = default;

    static Workspace& Instance();
	
private:
    std::map<std::string, ImageSession> _sessions;
    ImageSession _activeSession;

    static Workspace _instance;
};

#endif // WORKSPACE_H
