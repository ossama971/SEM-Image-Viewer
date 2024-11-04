#include "IMessage.h"

IMessage::IMessage(int id) : _id(id) {
}

IMessage::~IMessage(void) {
}

std::string IMessage::GetType(void) const {
	return _type;
}

std::string IMessage::GetMessage(void) const {
	return _msg;
}
