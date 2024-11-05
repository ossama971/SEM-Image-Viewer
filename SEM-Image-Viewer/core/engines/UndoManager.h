#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H

#include "../commands/ICommand.h"
#include "../filters/ImageFilter.h"
#include <stack>
#include <memory>

class UndoManager {
public:
    cv::Mat createFilterCommand(Image &image, std::unique_ptr<ImageFilter> filter,std::string description);

    void execute(std::unique_ptr<ICommand> cmd);
    cv::Mat undo();
    cv::Mat redo();
    void clearHistory();

private:
    std::stack<std::unique_ptr<ICommand>> _undo;
    std::stack<std::unique_ptr<ICommand>> _redo;
};

#endif // UNDO_MANAGER_H
