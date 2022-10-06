#include "SlurmJobSub.h"

#include <string>
#include <vector>

int main(int argc, char* argv[])
{
	SlurmJobSub sjs;

	std::vector<std::string> args;
	for(int i = 0; i < argc; i++)
	{
		args.push_back(argv[i]);
	}
	args.erase(args.begin());

	if(sjs.ShowHelp(args))return 0;
	if(sjs.Config(args[0], true))return 0;
	if(sjs.RecreateDirs())return 0;
	if(sjs.WriteJobs())return 0;

	return 0;
}
