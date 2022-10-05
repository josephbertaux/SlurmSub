#include "ShellCommandArg.h"

ShellCommandArg::ShellCommandArg()
{
	cmd = "";
	cmd_args.clear();
}

int ShellCommandArg::Config(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "ShellCommandArg::Config(std::vector<std::string> args):" << std::endl;

	uint u;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	cmd = args[0];
	for(u = 1; u < args.size(); u++)
	{
		cmd_args.push_back(args[u]);
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

std::string ShellCommandArg::Write()
{
	std::string s = cmd;
	for(uint u = 0; u < cmd_args.size(); u++)
	{
		s += " ";
		s += cmd_args[u];
	}

	return s;
}

std::string ShellCommandArg::Name()
{
	return cmd + "...";
}
