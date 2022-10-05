#include "SlurmJobSub.h"

//private member functions

void SlurmJobSub::MakeSJSMap()
{
	sjs_map.clear();
	std::string s = "";

	sjs_map["-h"] = 		SJS_TPL
					{
						s +
						"./slurm_sub ...\n" +
						"\tCreates SLURM jobs to parse data stored in source TFiles into target TFiles\n" + 
						"\tThis executable takes a single argument which is the name of a config file\n" +
						"\tThe lines of the config file have the following format:\n" +
						"\t\tfunc(args, ...);\n" +
						"\tWhere 'func' can take the following values:\n" +

						"\t\t'SetUserName'\n" +
						"\t\t'SetExecName'\n" +
						"\t\t'SetOutputDir'\n" +
						"\t\t'SetMaxJobs'*\n" +
						"\t\t'SetSubTimer'*\n" +

						"\tFuncs marked with * are optional to include in the config file\n" +
						"\tHowever, at least one of 'SetSourceName' or 'SetSourceList' must be specified\n" +
						"\tFor more information about any 'func', do:\n" +
						"\t\t./parse func\n" +
						"\t(The config file name cannot conflict with one of these options)\n",
						this,
						&SlurmJobSub::ShowHelp
					};

	sjs_map["SetUserName"] =	SJS_TPL
					{
						s +
						"SetUserName(std::string)\n" +
		   		    		"\tSet the username that will be used with SLURM for file submission\n",
						this,
						&SlurmJobSub::SetUserName
					};

	sjs_map["SetExecName"] =	SJS_TPL
					{
						s +
						"SetExecName(std::string)\n" +
		   		    		"\tSet the fullpath name of the executable that will be used with SLURM for file submission\n",
						this,
						&SlurmJobSub::SetExecName
					};

	sjs_map["SetOutputDir"] =	SJS_TPL
					{
						s +
						"SetOutputDir(std::string)\n" +
		       				"\tSet the full path name for the output directory\n" +
		       				"\tThe following will be recreated in that directory:\n" +
		       				"\t\tA subdir '_sub' containing the SLURM submission files\n" +
		       				"\t\tA subdir '_out' containing the outputs of the SLURM job(s)\n",
						this,
						&SlurmJobSub::SetOutputDir
					};

	sjs_map["SetMaxJobs"] =		SJS_TPL
					{
						s +
						"SetMaxJobs(int)\n" +
				       		"\tSets the maximum number of SLURM jobs that can be running concurrently in SLURM\n" +
				       		"\tThe program checks the number of jobs submitted by (SetUserName)\n" +
				       		"\t\tIf this is less than (SetMaxJobs), the program will submit jobs up to the difference\n" +
				       		"\t\tOtherwise, it waits an amount of time (SetSubTimer) before checking again\n",
						this,
						&SlurmJobSub::SetMaxJobs
					};


	sjs_map["SetSubTimer"] =	SlurmJobSub::SJS_TPL
					{
						s +
						"SetSubTimer(int)\n" +
						"\tSets the wait time (in seconds) between checks of the number of SLURM jobs\n" +
						"\tHas a default value of '60'\n",
						this,
						&SlurmJobSub::SetSubTimer
					};

	sjs_map["AddShellCommandArg"] =	SlurmJobSub::SJS_TPL
					{
						s +
						"AddShellCommandLineArg(int)\n" +
						"\tAdds a shell command line to be called in the SLURM sub file before the main program is ran\n" +
						"\tSince the Config(std::string) method removes whitespace characters,\n" + 
						"\tthe command and arguments must be separated with the delim character '" + delim + "' instead of spaces\n" +
						"\tOtherwise, the command is verbatim as it would be called in a shell session\n",
						this,
						&SlurmJobSub::AddShellCommandArg
					};

	sjs_map["AddCommandLineArg"] =	SlurmJobSub::SJS_TPL
					{
						s +
						"AddCommandLineArg(int)\n" +
						"\tAdds a command line arg to be passed to the executable being submitted to SLURM\n" +
						"\tThe first argument is the type of commandline arg\n" +
						"\t\t'i' is for CommandLineArg, the base class that corresponds to an integer value over a range\n" +
						"\t\t'v' is for CommandLineArgV, a child class that corresponds to a constant verbatim string\n" +
						"\t\t'f' is for CommandLineArgF, a child class that corresponds to a file that has its contents parsed in chunks of a given size\n" +
						"\tArguments are written after the executable in the order they are added\n",
						this,
						&SlurmJobSub::AddCommandLineArg
					};
}

//public member functions

SlurmJobSub::SlurmJobSub()
{
	MakeSJSMap();
}

SlurmJobSub::~SlurmJobSub()
{
	for(uint u = 0; u < cmd_line_args.size(); u++)
	{
		if(cmd_line_args[u])delete cmd_line_args[u];
	}
}

int SlurmJobSub::Implement(std::string func, std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "Implement(std::string func, std::vector<std::string> args):" << std::endl;

	SJS_MAP::const_iterator itr = sjs_map.find(func);
	if(itr == sjs_map.end())
	{
		output_str << "\tCould not find function '" << func << "'" << std::endl;
		return_val = 1;
		goto label;
	}
	if((std::get<1>(itr->second)->*std::get<2>(itr->second))(args))
	{
		output_str << "\tErrors implementing function '" << func << "'" << std::endl;
		return_val = 1;
		goto label;
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::Config(std::string config_file_name, bool v)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "Config(std::string config_file_name):" << std::endl;

	uint u;
	size_t pos;
	int line_num = 0;
	std::string line_str = "";
	std::string config_str = "";
	std::ifstream config_file;

	std::string func;
	std::vector<std::string> args;

	if(config_file_name == "")
	{
		output_str << "\tPassed argument 'config_file_name' was empty string" << std::endl;
		return_val = 1;
		goto label;
	}

	config_file.open(config_file_name, std::ifstream::in);
	if(!config_file.is_open())
	{
		output_str << "\tCould not get list:" << std::endl;
		output_str << "\t" << config_file_name << std::endl;
		return_val = 1;
		goto label;
	}

	output_str << "\tIn file:" << std::endl;
	output_str << "\t" << config_file_name << std::endl;

	config_str = "";
	line_str = "";
	while(true)
	{
		while(true)
		{
			if(config_file.bad())
			{
				output_str << "\tBad bit set" << std::endl;
				return_val = 1;
				goto label;
			}

			config_str += line_str;

			pos = config_str.find(closing_sym);
			if(pos != std::string::npos)
			{
				config_str = config_str.substr(0, pos + closing_sym.length());
				line_str = config_str.substr(pos + closing_sym.length());
				break;
			}

			std::getline(config_file, line_str);
			for(u = 0; u < cmmnts.size(); u++)
			{
				line_str = line_str.substr(0, line_str.find(cmmnts[u]));
			}
			line_num++;

			if(config_file.eof())
			{
				if(return_val)output_str << std::endl;
				output_str << "Reached end of config file" << std::endl;
				goto label;
			}
		}

		for(u = 0; u < prunes.size(); u++)
		{
			pos = config_str.find(prunes[u]);
			while(pos < config_str.length())
			{
				config_str.erase(pos, prunes[u].length());
				pos = config_str.find(prunes[u]);
			}
		}

		if(config_str.find(opening_sym) == std::string::npos)
		{
			output_str << "\tNo opening '" << opening_sym << "' found (line " << line_num << ")" << std::endl;
			return_val = 1;
		}
		if(config_str.find(closing_sym) == std::string::npos)
		{
			output_str << "\tNo closing '" << closing_sym << "' found (line " << line_num << ")" << std::endl;
			return_val = 1;
		}
		func = config_str.substr(0, config_str.find(opening_sym));
		config_str = config_str.substr(config_str.find(opening_sym) + opening_sym.length());
		config_str = config_str.substr(0, config_str.find(closing_sym));

		args.clear();
		pos = config_str.find(delim);
		while(pos < config_str.length())
		{
			args.push_back(config_str.substr(0, pos));
			config_str = config_str.substr(pos + delim.length());
			pos = config_str.find(delim);
		}
		if(config_str != "")args.push_back(config_str);

		if(v)
		{
			std::cout << "line " << line_num << ":" << std::endl;
			std::cout << "\t" << func;
			for(u = 0; u < args.size(); u++)
			{
				std::cout << " " << args[u];
			}
			std::cout << std::endl;
		}

		if(Implement(func, args))
		{
			std::cout << "\tline " << line_num << ":" << std::endl;
			std::cout << "\t" << func;
			for(u = 0; u < args.size(); u++)
			{
				std::cout << " " << args[u];
			}
			std::cout << std::endl;
			std::cout << std::endl;

			output_str << "\tProblem with config block ending on line: " << line_num << std::endl;
			return_val = 1;
		}

		config_str = "";
	}

	label:
	config_file.close();
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::RecreateDirs()
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "RecreateDirs():" << std::endl;

	if(std::filesystem::exists(output_dir.c_str()))
	{
		if(!std::filesystem::remove_all(output_dir.c_str()))
		{
			output_str << "\tFailed to remove dir:" << std::endl;
			output_str << "\t" << output_dir << std::endl;
			return_val = 1;
			goto label;
		}
	}

	if(!std::filesystem::exists(output_dir.c_str()))
	{
		if(!std::filesystem::create_directories(output_dir.c_str()))
		{
			output_str << "\tFailed to create dir:" << std::endl;
			output_str << "\t" << output_dir << std::endl;
			return_val = 1;
			goto label;
		}
	}

	if(!std::filesystem::exists((output_dir + "/_out").c_str()))
	{
		if(!std::filesystem::create_directories((output_dir + "/_out").c_str()))
		{
			output_str << "\tFailed to create subdir '_out' of dir:" << std::endl;
			output_str << "\t" << output_dir << std::endl;
			return_val = 1;
			goto label;
		}
	}

	if(!std::filesystem::exists((output_dir + "/_sub").c_str()))
	{
		if(!std::filesystem::create_directories((output_dir + "/_sub").c_str()))
		{
			output_str << "\tFailed to create subdir '_sub' of dir:" << std::endl;
			output_str << "\t" << output_dir << std::endl;
			return_val = 1;
			goto label;
		}
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::GetNumJobs(int& num_jobs)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "GetRunningJobs():" << std::endl;

	size_t pos;
	std::string str = "";
	str += "squeue -u ";
	str += user_name;
	char bffr[128];
	FILE* pipe = popen(str.c_str(), "r");

	num_jobs = max_jobs;

	if(!pipe)
	{
		output_str << "\tPipe is not open" << std::endl;
		return_val = 1;
		goto label;
	}

	//get the first line of the output to compare to the first line of SLURM's output
	str = "";
	while(fgets(bffr, sizeof(bffr), pipe) != NULL)
	{
		str += bffr;
		pos = str.find("\n");
		if(pos != std::string::npos)break;
	}
	if(str.substr(0, pos) != "JOBID        USER      ACCOUNT      NAME             NODES   CPUS  TIME_LIMIT ST TIME")
	{
		output_str << "\tsqueue command fails to retrieve expected output" << std::endl;
		return_val = 1;
		goto label;
	}

	num_jobs = 0;

	str = str.substr(pos + 1);
	pos = str.find("\n");
	while(pos < str.length())
	{
		num_jobs++;
		str = str.substr(pos + 1);
		pos = str.find("\n");
	}
	while(fgets(bffr, sizeof(bffr), pipe) != NULL)
	{
		str = bffr;
		pos = str.find("\n");
		while(pos < str.length())
		{
			num_jobs++;
			str = str.substr(pos + 1);
			pos = str.find("\n");
		}
	}

	label:
	pclose(pipe);
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::WriteJob(std::string& sub_file_name)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "WriteJob(std::string& file_name):" << std::endl;

	uint u;
	size_t pos;
	std::string temp;
	std::string out_file_name;
	std::ofstream sub_file;

	temp = CommandLineArgF::CleanFileArg(exec_name);
	for(u = 0; u < cmd_line_args.size(); u++)
	{
		temp += "_";
		temp += cmd_line_args[u]->Name();
	}

	sub_file_name = output_dir + "/_sub/" + temp + ".sub";
	out_file_name = output_dir + "/_out/" + temp + ".out";

	sub_file.open(sub_file_name.c_str(), std::ofstream::out | std::ofstream::trunc);
	if(!sub_file.is_open())
	{
		output_str << "\tFailed to create SLURM sub file:" << std::endl;
		output_str << "\t" << sub_file_name << std::endl;
		return_val = 1;
		goto label;
	}

	sub_file << "#!/bin/bash" << std::endl;
	sub_file << "#SBATCH --nodes=1" << std::endl;
	sub_file << "#SBATCH -o " << out_file_name << std::endl;
	sub_file << "#SBATCH --job-name='" << temp << "'" << std::endl;
	sub_file << "#SBATCH --no-requeue" << std::endl;
	sub_file << "#SBATCH --export=ALL" << std::endl;

	for(u = 0; u < shell_cmd_args.size(); u++)
	{
		sub_file << shell_cmd_args[u]->Write() << std::endl;
	}

	temp = exec_name;
	for(u = 0; u < cmd_line_args.size(); u++)
	{
		temp += " ";
		temp += cmd_line_args[u]->Write();
	}
	sub_file << temp << std::endl;
	sub_file.close();

	temp = "sbatch --time=04:00:00 -A standby ";
	temp += sub_file_name;
	std::system(temp.c_str());

	label:
	sub_file.close();
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::WriteJobs()
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "WriteJobs():" << std::endl;

	uint u;
	int n;
	int N;
	int jobs;
	int current_arg = 0;
	std::string s = "";
	std::time_t t;

	if(max_jobs < 1)
	{
		output_str << "\tMember 'max_jobs' is less than 1" << std::endl;
		return_val = 1;
		goto label;
	}

	jobs = 0;
	while(true)
	{
		if(GetNumJobs(N))break;
		N = max_jobs - N;

		for(n = 0; n < N; n++)
		{
			if(WriteJob(s))
			{
				output_str << "\tProblem writing submission file:" << std::endl;
				output_str << "\t" << s << std::endl;
				return_val = 1;
				goto label;
			}
			jobs++;
			current_arg = cmd_line_args.size() - 1;
			if(current_arg < 0)goto label;
			while(cmd_line_args[current_arg]->Increment())
			{
				current_arg--;
				if(current_arg < 0)
				{
					t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
					std::cout << std::ctime(&t);
					std::cout << "Submitted batch of " << n + 1 << " jobs (" << jobs << " total)" << std::endl;
					std::cout << "Done" << std::endl;
					goto label;
				}
			}
		}

		t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::cout << std::ctime(&t);
		std::cout << "Submitted batch of " << n << " jobs (" << jobs << " total)" << std::endl;
		std::cout << "Waiting " << sub_timer << " seconds" << std::endl;
		std::cout << std::endl;
		std::this_thread::sleep_for(std::chrono::seconds(sub_timer));
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

//member access functions
int SlurmJobSub::ShowHelp(std::vector<std::string>)
{
	return 0;
}

int SlurmJobSub::SetUserName(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetUserName(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}
	
	if(args[0] == "")
	{
		output_str << "\tPassed argument 'args' 0th element is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	user_name = args[0];

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::SetExecName(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetExecName(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}
	
	if(args[0] == "")
	{
		output_str << "\tPassed argument 'args' 0th element is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	exec_name = args[0];

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::SetOutputDir(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetOutputDir(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}
	
	if(args[0] == "")
	{
		output_str << "\tPassed argument 'args' 0th element is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	output_dir = args[0];

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::SetMaxJobs(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetMaxJobs(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	try
	{
		max_jobs = std::stoi(args[0]);
	}
	catch(const std::invalid_argument&)
	{
		output_str << "\tstd::stoi failed to deduce '" << args[0] << "' as int" << std::endl;
		return_val = 1;
		goto label;
	}

	if(max_jobs < 1)max_jobs = 1;

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::SetSubTimer(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "SetSubTimer(std::vector<std::string> args):" << std::endl;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	try
	{
		sub_timer = std::stoi(args[0]);
	}
	catch(const std::invalid_argument&)
	{
		output_str << "\tstd::stoi failed to deduce '" << args[0] << "' as int" << std::endl;
		return_val = 1;
		goto label;
	}

	label:
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::AddShellCommandArg(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddShellCommandArg(std::vector<std::string> args):" << std::endl;

	ShellCommandArg* arg = nullptr;
	std::string type;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	arg = new ShellCommandArg();
	//...

	if(arg->Config(args))
	{
		output_str << "\tProblem adding ShellCommandArg" << std::endl;
		return_val = 1;
		goto label;
	}

	shell_cmd_args.push_back(arg);

	label:
	if(return_val and arg)delete arg;
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}

int SlurmJobSub::AddCommandLineArg(std::vector<std::string> args)
{
	int return_val = 0;
	std::stringstream output_str;
	output_str << "AddCommandLineArg(std::vector<std::string> args):" << std::endl;

	CommandLineArg* arg = nullptr;
	std::string type;

	if(args.size() == 0)
	{
		output_str << "\tPassed argument 'args' is empty" << std::endl;
		return_val = 1;
		goto label;
	}

	type = args[0];
	args.erase(args.begin());

	if(type == "i")arg = new CommandLineArg();
	if(type == "f")arg = new CommandLineArgF();
	//...

	if(!arg)
	{
		output_str << "\tType indicator '" << type << "' unrecognized, no arg added" << std::endl;
		return_val = 1;
		goto label;
	}
	if(arg->Config(args))
	{
		output_str << "\tProblem adding CommandLineArg" << std::endl;
		return_val = 1;
		goto label;
	}

	cmd_line_args.push_back(arg);

	label:
	if(return_val and arg)delete arg;
	output_str << std::ends;
	if(return_val)std::cout << output_str.str();
	return return_val;
}
