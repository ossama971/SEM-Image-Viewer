#include "FilterCommand.h"

FilterCommand::FilterCommand(std::unique_ptr<ImageFilter> filter) : _image(nullptr), _filter(std::move(filter)) {
}

FilterCommand::~FilterCommand()
{
}

void FilterCommand::setImage(Image* image) {
    if (!image)
        return;

    _image = image;
}

cv::Mat FilterCommand::execute() {
    return _filter->applyFilter(*_image);
}

//cv::Mat FilterCommand::undo()
//{
//    return _previuos;
//}


//std::string FilterCommand::getDescription() const
//{
//    return _description;
//}
