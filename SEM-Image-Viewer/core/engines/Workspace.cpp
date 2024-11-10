#include "Workspace.h"

Workspace Workspace::_instance;

SessionData& Workspace::getActiveSession() {
    return _activeSession;
}

Workspace& Workspace::Instance(void) {
	return _instance;
}
