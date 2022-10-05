#ifndef COMMAND_LINE_ARGF_H
#define COMMAND_LINE_ARGF_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

#include "CommandLineArg.h"

class CommandLineArgF: public CommandLineArg
{
protected:
	std::string file_name;
	int SetStoppingIndex(std::string) override;

public:
	using CommandLineArg::CommandLineArg;

	int Config(std::vector<std::string>) override;
	std::string Write() override;
	std::string Name() override;
};

#endif
