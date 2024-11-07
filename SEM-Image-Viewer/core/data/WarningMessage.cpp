#include "WarningMessage.h"

WarningMessage::WarningMessage(int msgId, const boost::format msg) : IMessage(msgId) {
    _msg = msg;
    _type = MessageLevel::WARNING;
}
