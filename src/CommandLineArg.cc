#include "CommandLineArg.h"

CommandLineArg::CommandLineArg()
{
	//Do nothihng
}

int CommandLineArg::Config(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArg::Config(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int CommandLineArg::Increment()
{
	return 1;
}

std::string CommandLineArg::Write()
{
	return "";
}

std::string CommandLineArg::Name()
{
	return CleanFileArg(Write());
}

std::string CommandLineArg::CleanFileArg(std::string s)
{
	size_t pos;

	pos = s.find("/");
	while(pos < s.length())
	{
		s = s.substr(pos + 1);
		pos = s.find("/");
	}

	pos = s.find(" ");
	while(pos < s.length())
	{
		s.replace(pos, 1, "_");
		pos = s.find(" ");
	}
	pos = s.find(".");
	while(pos < s.length())
	{
		s.replace(pos, 1, "_");
		pos = s.find(".");
	}

	return s;
}
