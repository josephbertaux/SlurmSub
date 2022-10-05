#include "SlurmJobSub.h"

#include <iostream>
#include <stdexcept>
#include <stdio.h>
#include <string>


int main()
{
	SlurmJobSub sjs;

	sjs.Config("/home/jbertau/Data/Repositories/SlurmSub/example.txt", true);
	int n;

	sjs.GetNumJobs(n);

	sjs.RecreateDirs();
	sjs.WriteJobs();

	return 0;
}
