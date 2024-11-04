#ifndef FILTER_COMMAND_H
#define FILTER_COMMAND_H

#include "ICommand.h"
#include "../filters/ImageFilter.h"
#include <memory>

class FilterCommand : public ICommand {
public:
    FilterCommand(cv::Mat image, std::unique_ptr<ImageFilter> filter);
    virtual ~FilterCommand();
	
public:
    void execute() override;
    void undo() override;
	
    std::string getDescription() const override;

private:
    cv::Mat _image;
    std::unique_ptr<ImageFilter> _filter;
};

#endif // FILTER_COMMAND_H
