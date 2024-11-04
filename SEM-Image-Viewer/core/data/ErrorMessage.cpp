#include "ErrorMessage.h"

ErrorMessage::ErrorMessage(int msgId, const std::string msg) : IMessage(msgId) {
    _msg = msg;
    _type = "Error";
}
