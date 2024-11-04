#ifndef WARNING_MESSAGE_H
#define WARNING_MESSAGE_H

#include "IMessage.h"

class WarningMessage : public IMessage {
    WarningMessage(int msgId, const std::string msg);
};

#endif // WARNING_MESSAGE_H
