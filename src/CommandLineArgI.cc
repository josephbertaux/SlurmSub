#include "CommandLineArgI.h"

CommandLineArgI::CommandLineArgI()
{
	current_index = 0;
	starting_index = 0;
	stopping_index = 1;
	stepping_index = 1;
}

int CommandLineArgI::SetStartingIndex(std::string s)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArgI::SetStartingIndex(std::vector<std::string> s):" << std::endl;

	if(s == "")
	{
		output_str << "\tPassed argument s is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	try
	{
		starting_index = std::stoi(s);
	}
	catch(const std::invalid_argument&)
	{
		output_str << "\tstd::stoi failed to deduce '" << s << "' as int" << std::endl;
		return_val = 1;
		goto label;
	}

	current_index = starting_index;

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int CommandLineArgI::SetStoppingIndex(std::string s)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArgI::SetStoppingIndex(std::vector<std::string> s):" << std::endl;

	if(s == "")
	{
		output_str << "\tPassed argument s is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	try
	{
		stopping_index = std::stoi(s);
	}
	catch(const std::invalid_argument&)
	{
		output_str << "\tstd::stoi failed to deduce '" << s << "' as int" << std::endl;
		output_str << "\ttrying as file" << std::endl;
		goto label;
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int CommandLineArgI::SetSteppingIndex(std::string s)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArgI::SetSteppingIndex(std::vector<std::string> s):" << std::endl;

	if(s == "")
	{
		output_str << "\tPassed argument s is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	try
	{
		stepping_index = std::stoi(s);
	}
	catch(const std::invalid_argument&)
	{
		output_str << "\tstd::stoi failed to deduce '" << s << "' as int" << std::endl;
		return_val = 1;
		goto label;
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int CommandLineArgI::Config(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArgI::Config(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}
	if(args.size() == 1)
	{
		if(SetStoppingIndex(args[0]))return_val = 1;
	}
	if(args.size() >= 2)
	{
		if(SetStartingIndex(args[0]))return_val = 1;
		if(SetStoppingIndex(args[1]))return_val = 1;
	}
	if(args.size() >= 3)
	{
		if(SetSteppingIndex(args[2]))return_val = 1;
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int CommandLineArgI::Increment()
{
	current_index += stepping_index;
	if(current_index >= stopping_index)
	{
		current_index = starting_index;
		return 1;
	}

	return 0;
}

std::string CommandLineArgI::Write()
{
	return std::to_string(current_index);
}

std::string CommandLineArgI::Name()
{
	return CleanFileArg(Write());
}
