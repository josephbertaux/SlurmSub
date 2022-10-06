#ifndef COMMAND_LINE_ARG_H
#define COMMAND_LINE_ARG_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class CommandLineArg
{
protected:

public:
	CommandLineArg();

	virtual int Config(std::vector<std::string>);
	virtual int Increment();
	virtual std::string Write();
	virtual std::string Name();

	static std::string CleanFileArg(std::string);
};

#endif
