#ifndef INFO_MESSAGE
#define INFO_MESSAGE

#include "IMessage.h"

class InfoMessage : public IMessage {
public:
    InfoMessage(int msgId, const boost::format msg);
};

#endif // INFO_MESSAGE
