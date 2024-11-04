#ifndef ICOMMAND_H
#define ICOMMAND_H

#include <iostream>

class ICommand {
protected:
	ICommand() = default;
public:
	virtual ~ICommand() = default;
	
public:

    virtual void execute() = 0;
	virtual void undo() = 0;
	
	virtual std::string getDescription() const = 0;
};

#endif // ICOMMAND_H
