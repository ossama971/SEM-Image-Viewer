#include "UndoManager.h"
#include "../commands/FilterCommand.h"

std::unique_ptr<ICommand> UndoManager::createFilterCommand(const cv::Mat &image, std::unique_ptr<ImageFilter> filter)
{
    return std::make_unique<FilterCommand>(image, std::move(filter));
}
