#include "IMessage.h"

IMessage::IMessage(int id) : _id(id) {
}

IMessage::~IMessage(void) {
}

int IMessage::GetId(void) const {
  return _id;
}

std::string IMessage::GetType(void) const {
  switch (_type) {
  case MessageLevel::ERROR:
    return "Error";
  case MessageLevel::WARNING:
    return "Warning";
  case MessageLevel::INFO:
    return "Info";
  case MessageLevel::DEBUG:
    return "Debug";
  case MessageLevel::TRACE:
    return "Trace";
  default:
    return "Unknown";
  }
}

std::string IMessage::GetMessage(void) const {
	return _msg.str();
}

std::string IMessage::asString(void) const {
  return (boost::format("[%1%] %2%") % GetType() % GetMessage()).str();
}
