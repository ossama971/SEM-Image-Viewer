#ifndef IMESSAGE_H
#define IMESSAGE_H

#include <string>
#include <boost/format.hpp>

class IMessage {
public:
  enum class MessageLevel : uint8_t {
    ERROR = 1,
    WARNING,
    INFO,
    DEBUG,
    TRACE
  };

protected:
  IMessage(int msgId);

public:
  virtual ~IMessage();

  virtual int GetId() const;
  virtual std::string GetType() const;
  virtual std::string GetMessage() const;
  virtual std::string asString() const;

protected:
  int _id;
  MessageLevel _type;
  boost::format _msg;
};

#endif // IMESSAGE_H
