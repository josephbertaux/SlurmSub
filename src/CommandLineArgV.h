#ifndef COMMAND_LINE_ARG_V_H
#define COMMAND_LINE_ARG_V_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "CommandLineArg.h"

class CommandLineArgV: public CommandLineArg
{
protected:
	std::string v;

public:
	using CommandLineArg::CommandLineArg;

	CommandLineArgV();

	virtual int Config(std::vector<std::string>) override;
	virtual std::string Write() override;
	virtual std::string Name() override;
};

#endif
