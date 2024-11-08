#ifndef ERROR_MESSAGE_H
#define ERROR_MESSAGE_H

#include "IMessage.h"
#include <boost/format.hpp>

class ErrorMessage : public IMessage {
public:
    ErrorMessage(int msgId, const boost::format msg);
};

#endif // ERROR_MESSAGE_H
