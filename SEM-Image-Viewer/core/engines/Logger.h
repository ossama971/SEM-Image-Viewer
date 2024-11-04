#ifndef LOGGER_H
#define LOGGER_H

#include "../data/IMessage.h"
#include <vector>
#include <memory>
#include <atomic>

class Logger
{
private:
    Logger() = default;
public:
    ~Logger() = default;

    void createInfoMsg(std::string msg);
    void createWarningMsg(std::string msg);
    void createErrorMsg(std::string msg);

private:
    void add(std::unique_ptr<IMessage> msg);
public:
    void remove(int msgId);

    std::vector<std::unique_ptr<IMessage>> getMsgs();
    static Logger& Instance();
	
private:
    std::vector<std::unique_ptr<IMessage>> _msgs;
    std::atomic<int> _nextMsgId;

    static Logger _instance;
};

#endif // LOGGER_H
