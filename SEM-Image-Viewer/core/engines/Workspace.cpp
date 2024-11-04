#include "Workspace.h"

Workspace Workspace::_instance;

Workspace& Workspace::Instance(void) {
	return _instance;
}
