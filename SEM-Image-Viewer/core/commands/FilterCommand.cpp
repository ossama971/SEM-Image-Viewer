#include "FilterCommand.h"

FilterCommand::FilterCommand(cv::Mat image, std::unique_ptr<ImageFilter> filter) : _image(image), _filter(std::move(filter)) {
}

FilterCommand::~FilterCommand()
{
}

void FilterCommand::execute()
{

}

void FilterCommand::undo()
{

}

std::string FilterCommand::getDescription() const
{
    return std::string();
}
