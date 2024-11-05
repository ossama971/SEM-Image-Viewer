#include "FilterCommand.h"

FilterCommand::FilterCommand(Image &image, std::unique_ptr<ImageFilter> filter,std::string description ) : _image(image), _filter(std::move(filter)),_description(description) {
}

FilterCommand::~FilterCommand()
{
}

cv::Mat FilterCommand::execute()
{
    _previuos=_image.getImageMat();
    return _filter->applyFilter(_image);
}

 cv::Mat FilterCommand::undo()
{

    return _previuos;
}


std::string FilterCommand::getDescription() const
{
    return _description;
}
