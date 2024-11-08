#include "InfoMessage.h"

InfoMessage::InfoMessage(int msgId, const boost::format msg) : IMessage(msgId) {
    _msg = msg;
    _type = MessageLevel::INFO;
}
