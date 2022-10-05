#ifndef SHELL_COMMAND_ARG_H
#define SHELL_COMMAND_ARG_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class ShellCommandArg
{
protected:
	std::string cmd;
	std::vector<std::string> cmd_args;

public:
	ShellCommandArg();

	virtual int Config(std::vector<std::string>);
	virtual std::string Write();
	virtual std::string Name();
};

#endif
