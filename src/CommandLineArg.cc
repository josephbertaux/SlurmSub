#include "CommandLineArg.h"

CommandLineArg::CommandLineArg()
{
	current_index = 0;
	starting_index = 0;
	stopping_index = 1;
	stepping_index = 1;
}

int CommandLineArg::SetStartingIndex(std::string s)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArg::SetStartingIndex(std::vector<std::string> s):" << std::endl;

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

int CommandLineArg::SetStoppingIndex(std::string s)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArg::SetStoppingIndex(std::vector<std::string> s):" << std::endl;

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

int CommandLineArg::SetSteppingIndex(std::string s)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArg::SetSteppingIndex(std::vector<std::string> s):" << std::endl;

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

int CommandLineArg::Increment()
{
	current_index += stepping_index;
	if(current_index >= stopping_index)
	{
		current_index = starting_index;
		return 1;
	}

	return 0;
}

std::string CommandLineArg::Write()
{
	return std::to_string(current_index);
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
