#include "ErrorMessage.h"

ErrorMessage::ErrorMessage(int msgId, const boost::format msg) : IMessage(msgId) {
    _msg = msg;
    _type = MessageLevel::ERROR;
}
