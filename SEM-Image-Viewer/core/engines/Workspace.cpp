#include "Workspace.h"

Workspace Workspace::_instance;

ImageSession& Workspace::getActiveSession() {
    return _activeSession;
}

Workspace& Workspace::Instance(void) {
	return _instance;
}
