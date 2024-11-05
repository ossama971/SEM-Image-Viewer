#ifndef FILTER_COMMAND_H
#define FILTER_COMMAND_H

#include "ICommand.h"
#include "../filters/ImageFilter.h"
#include "../data/Image.h"
#include <memory>

class FilterCommand : public ICommand {
public:
    FilterCommand(Image &image, std::unique_ptr<ImageFilter> filter,std::string description);
    virtual ~FilterCommand();
	
public:
    cv::Mat execute() override;
    cv::Mat undo() override;
	
    std::string getDescription() const override;

private:
    Image _image;
    cv::Mat _previuos;
    std::string _description;
    std::unique_ptr<ImageFilter> _filter;
};

#endif // FILTER_COMMAND_H
