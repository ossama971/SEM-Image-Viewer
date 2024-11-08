#ifndef WARNING_MESSAGE_H
#define WARNING_MESSAGE_H

#include "IMessage.h"
#include <boost/format.hpp>

class WarningMessage : public IMessage {
public:
    WarningMessage(int msgId, const boost::format msg);
};

#endif // WARNING_MESSAGE_H
