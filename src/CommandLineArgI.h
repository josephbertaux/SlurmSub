#ifndef COMMAND_LINE_ARG_I_H
#define COMMAND_LINE_ARG_I_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "CommandLineArg.h"

class CommandLineArgI: public CommandLineArg
{
protected:
	int current_index;

	int starting_index;
	int stopping_index;
	int stepping_index;

	virtual int SetStartingIndex(std::string);
	virtual int SetStoppingIndex(std::string);
	virtual int SetSteppingIndex(std::string);

public:
	using CommandLineArg::CommandLineArg;

	CommandLineArgI();

	virtual int Config(std::vector<std::string>) override;
	virtual int Increment() override;
	virtual std::string Write() override;
	virtual std::string Name() override;
};

#endif
