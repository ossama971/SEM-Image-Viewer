#ifndef INFO_MESSAGE
#define INFO_MESSAGE

#include "IMessage.h"

class InfoMessage : public IMessage {
    InfoMessage(int msgId, const std::string msg);
};

#endif // INFO_MESSAGE
