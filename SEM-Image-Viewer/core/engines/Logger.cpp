#include "Logger.h"

Logger Logger::_instance;

Logger& Logger::Instance(void) {
	return _instance;
}
