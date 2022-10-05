#ifndef SLURM_JOB_SUB_H
#define SLURM_JOB_SUB_H

#include <cstdlib>

#include <map>
#include <tuple>
#include <string>
#include <cstring>
#include <vector>

#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include <thread>
#include <chrono>
#include <ctime>

#include <stdexcept>
//...

#include "ShellCommandArg.h"

#include "CommandLineArg.h"
#include "CommandLineArgF.h"

class SlurmJobSub
{
public:
	typedef int(SlurmJobSub::*SJS_MMBR_FUNC)(std::vector<std::string>);
	typedef std::tuple<std::string, SlurmJobSub*, SJS_MMBR_FUNC> SJS_TPL;
	typedef std::map<std::string, SJS_TPL> SJS_MAP;

protected:
	SJS_MAP sjs_map;
	void MakeSJSMap();

	std::vector<std::string> cmmnts = {"//", "#"};
	std::vector<std::string> prunes = {" ", "\t", "\n"};

	std::string opening_sym = "{";
	std::string closing_sym = "}";
	std::string delim = ";";

	std::vector<ShellCommandArg*> shell_cmd_args = {};
	std::vector<CommandLineArg*> cmd_line_args = {};

	std::string user_name = "";
	std::string exec_name = "";
	std::string output_dir = "";
	int max_jobs = 1;
	int sub_timer = 0;
public:

	SlurmJobSub();
	~SlurmJobSub();

	int Implement(std::string, std::vector<std::string>);
	int Config(std::string, bool);

	int RecreateDirs();
	int GetNumJobs(int&);
	int WriteJob(std::string&);
	int WriteJobs();

	int ShowHelp(std::vector<std::string>);
	int SetUserName(std::vector<std::string>);
	int SetExecName(std::vector<std::string>);
	int SetOutputDir(std::vector<std::string>);
	int SetMaxJobs(std::vector<std::string>);
	int SetSubTimer(std::vector<std::string>);
	int AddShellCommandArg(std::vector<std::string>);
	int AddCommandLineArg(std::vector<std::string>);
};

#endif
