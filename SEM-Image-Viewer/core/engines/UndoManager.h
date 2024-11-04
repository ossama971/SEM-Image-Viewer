#ifndef UNDO_MANAGER_H
#define UNDO_MANAGER_H

#include "../commands/ICommand.h"
#include "../filters/ImageFilter.h"
#include <stack>
#include <memory>

class UndoManager {
public:
    std::unique_ptr<ICommand> createFilterCommand(const cv::Mat &image, std::unique_ptr<ImageFilter> filter);

    void execute(std::unique_ptr<ICommand> cmd);
    void undo();
    void redo();
    void clearHistory();

private:
    std::stack<std::unique_ptr<ICommand>> _undo;
    std::stack<std::unique_ptr<ICommand>> _redo;
};

#endif // UNDO_MANAGER_H
