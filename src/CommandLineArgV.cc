#include "CommandLineArgV.h"

CommandLineArgV::CommandLineArgV()
{
	v = "";
}

int CommandLineArgV::Config(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArgV::Config(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	v= args[0];

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

std::string CommandLineArgV::Write()
{
	return v;
}

std::string CommandLineArgV::Name()
{
	return CleanFileArg(Write());
}
