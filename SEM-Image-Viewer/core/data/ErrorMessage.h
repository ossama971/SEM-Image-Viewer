#ifndef ERROR_MESSAGE_H
#define ERROR_MESSAGE_H

#include "IMessage.h"

class ErrorMessage : public IMessage {
    ErrorMessage(int msgId, const std::string msg);
};

#endif // ERROR_MESSAGE_H
