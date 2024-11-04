#ifndef IMESSAGE_H
#define IMESSAGE_H

#include <string>

class IMessage {
protected:
    IMessage(int msgId);
public:
    virtual ~IMessage();

    int GetId() const;
	std::string GetType() const;
	std::string GetMessage() const;
	
protected:
    int _id;
	std::string _type;
	std::string _msg;
};

#endif // IMESSAGE_H
