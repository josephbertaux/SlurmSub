#include "CommandLineArgF.h"

CommandLineArgF::CommandLineArgF()
{
	file_name = "";
}

int CommandLineArgF::SetStoppingIndex(std::string s)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArgF::SetStoppingIndex(std::vector<std::string> s):" << std::endl;

	size_t pos = 0;
	std::string str = "";
	char bffr[128];
	FILE* file = nullptr;

	stopping_index = 0;
	file = fopen(s.c_str(), "r");

	if(!file)
	{
		output_str << "\tFailed to open file:" << std::endl;
		output_str << "\t" << s << std::endl;
		return_val = 1;
		goto label;
	}

	while(fgets(bffr, sizeof(bffr), file) != NULL)
	{
		str = bffr;
		pos = str.find("\n");
		while(pos < str.length())
		{
			stopping_index++;
			str = str.substr(pos + 1);
			pos = str.find("\n");
		}
	}

	goto label;

	label:
	if(file)fclose(file);
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int CommandLineArgF::Config(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "CommandLineArgF::Config(std::vector<std::string> args):" << std::endl;


	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	file_name = args[0];	
	if(SetStoppingIndex(args[0]))return_val = 1;

	if(args.size() == 2)
	{
		if(SetSteppingIndex(args[1]))return_val = 1;
	}
	if(args.size() >= 3)
	{
		if(SetStartingIndex(args[1]))return_val = 1;
		if(SetStoppingIndex(args[2]))return_val = 1;
	}
	if(args.size() >= 4)
	{
		if(SetSteppingIndex(args[3]))return_val = 1;
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

std::string CommandLineArgF::Write()
{
	return file_name + " " + std::to_string(current_index) + " " + std::to_string(std::min(current_index + stepping_index, stopping_index));
}

std::string CommandLineArgF::Name()
{
	return CleanFileArg(Write());
}
