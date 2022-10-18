#!/bin/bash

if [ $# -lt 1 ]
then
	echo -e "\n"
	echo -e "Must pass one argument, the name of the config file to use with slurm_sub"
	echo -e "The argument passed to this script is then passed to ths slurm_sub"
	echo -e "And the submission is done with nohup"
	echo -e "Two files are written in the directory where this script is called:"
	echo -e "\t'nohup_slurm.out', which saves the job outpud"
	echo -e "\t'nohup_slurm.pid', which saves the job's pid"
	echo -e "\n"

	exit 0
fi

nohup $( dirname -- "$0"; )/slurm_sub $1 > nohup_slurm.out &
echo $! > nohup_slurm.pid

exit 0
