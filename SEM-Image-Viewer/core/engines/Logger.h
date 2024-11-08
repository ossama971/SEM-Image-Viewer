#ifndef LOGGER_H
#define LOGGER_H

#include "../data/IMessage.h"
#include "../../models/MessageDataModel.h"

#include <memory>
#include <mutex>

class Logger
{
public:
    virtual ~Logger() = default;
    void log(std::unique_ptr<IMessage> msg);
    void setModel(std::shared_ptr<MessageDataModel> model);
    static std::unique_ptr<Logger>& instance();

private:
    Logger() = default;

private:
    std::shared_ptr<MessageDataModel> m_dataModel;
    static std::unique_ptr<Logger> m_instance;
    static std::mutex m_mutex;
};

#endif // LOGGER_H
