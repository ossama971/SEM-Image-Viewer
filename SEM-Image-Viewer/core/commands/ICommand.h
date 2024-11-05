#ifndef ICOMMAND_H
#define ICOMMAND_H


#include <opencv2/opencv.hpp>

class ICommand {
protected:
	ICommand() = default;
public:
	virtual ~ICommand() = default;
	
public:

    virtual cv::Mat execute() = 0;
    virtual cv::Mat undo() = 0;
	
	virtual std::string getDescription() const = 0;
};

#endif // ICOMMAND_H
