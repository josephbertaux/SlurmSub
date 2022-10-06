#ifndef COMMAND_LINE_ARG_F_H
#define COMMAND_LINE_ARG_F_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "CommandLineArgI.h"

class CommandLineArgF: public CommandLineArgI
{
protected:
	std::string file_name;
	int SetStoppingIndex(std::string) override;

public:
	using CommandLineArgI::CommandLineArgI;

	CommandLineArgF();

	int Config(std::vector<std::string>) override;
	std::string Write() override;
	std::string Name() override;
};

#endif
