#include "WarningMessage.h"

WarningMessage::WarningMessage(int msgId, const std::string msg) : IMessage(msgId) {
    _msg = msg;
    _type = "Warning";
}
