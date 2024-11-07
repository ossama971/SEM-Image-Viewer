#ifndef FILTER_COMMAND_H
#define FILTER_COMMAND_H

#include "ICommand.h"
#include "../filters/ImageFilter.h"
#include "../data/Image.h"
#include <memory>

class FilterCommand : public ICommand {
public:
    FilterCommand(std::unique_ptr<ImageFilter> filter);
    virtual ~FilterCommand();
	
public:
    void setImage(Image* image);

    cv::Mat execute() override;
    //cv::Mat undo() override;
	
    //std::string getDescription() const override;

private:
    Image* _image;
    std::unique_ptr<ImageFilter> _filter;
    //std::string _description;
};

#endif // FILTER_COMMAND_H
