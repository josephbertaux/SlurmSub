#ifndef COMMAND_LINE_ARG_H
#define COMMAND_LINE_ARG_H

#include <string>
#include <vector>
#include <iostream>
#include <sstream>

class CommandLineArg
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
	CommandLineArg();

	virtual int Config(std::vector<std::string>);
	virtual int Increment();
	virtual std::string Write();
	virtual std::string Name();

	static std::string CleanFileArg(std::string);
};

#endif
