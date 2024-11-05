#include "UndoManager.h"
#include "../commands/FilterCommand.h"

cv::Mat UndoManager::createFilterCommand(Image &image, std::unique_ptr<ImageFilter> filter,std::string description)
{
    auto command=std::make_unique<FilterCommand>(image, std::move(filter),description);
    auto _mat=command->execute();
    _undo.push(std::move(command));
    return _mat;
}



cv::Mat UndoManager::undo()
{
    ICommand * command =_undo.top().get();
    auto _res=command->undo();
    _redo.push(std::move(_undo.top()));
    _undo.pop();
    return _res;
}


cv::Mat UndoManager::redo()
{
    ICommand * command =_redo.top().get();
    auto _res=command->undo();
    _undo.push(std::move(_redo.top()));
    _redo.pop();
    return _res;
}


void UndoManager::clearHistory(){
    while(_redo.size()){
        _redo.pop();
    }
    while(_undo.size()){
        _undo.pop();
    }
}
