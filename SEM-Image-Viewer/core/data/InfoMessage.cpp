#include "InfoMessage.h"

InfoMessage::InfoMessage(int msgId, const std::string msg) : IMessage(msgId) {
    _msg = msg;
    _type = "Info";
}
