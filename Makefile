SHELL=/bin/bash

TRGT = slurm_sub

SRC_DIR = ./src
OBJ_DIR = ./bld

CC = g++

OBJS = $(OBJ_DIR)/SlurmSub.o $(OBJ_DIR)/SlurmJobSub.o $(OBJ_DIR)/ShellCommandArg.o $(OBJ_DIR)/CommandLineArg.o \
	$(OBJ_DIR)/CommandLineArgV.o $(OBJ_DIR)/CommandLineArgI.o $(OBJ_DIR)/CommandLineArgF.o

FLAGS = `root-config --cflags`
#INCS = -I`root-config --incdir` #already included with root-config --cflags
LIBS = `root-config --evelibs` -lTMVA -lTMVAGui -lRooFitCore -lRooFitMore -lRooFit

$(TRGT) : $(OBJS)
	$(CC) $(FLAGS) $(INCS) -o slurm_sub $(OBJS) $(LIBS)

$(OBJ_DIR)/SlurmSub.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/SlurmSub.cc $(LIBS)
	mv SlurmSub.o $(OBJ_DIR)/SlurmSub.o

$(OBJ_DIR)/SlurmJobSub.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/SlurmJobSub.cc $(LIBS)
	mv SlurmJobSub.o $(OBJ_DIR)/SlurmJobSub.o

$(OBJ_DIR)/ShellCommandArg.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/ShellCommandArg.cc $(LIBS)
	mv ShellCommandArg.o $(OBJ_DIR)/ShellCommandArg.o

$(OBJ_DIR)/CommandLineArg.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/CommandLineArg.cc $(LIBS)
	mv CommandLineArg.o $(OBJ_DIR)/CommandLineArg.o

$(OBJ_DIR)/CommandLineArgV.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/CommandLineArgV.cc $(LIBS)
	mv CommandLineArgV.o $(OBJ_DIR)/CommandLineArgV.o

$(OBJ_DIR)/CommandLineArgI.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/CommandLineArgI.cc $(LIBS)
	mv CommandLineArgI.o $(OBJ_DIR)/CommandLineArgI.o

$(OBJ_DIR)/CommandLineArgF.o :
	$(CC) $(FLAGS) $(INCS) -c $(SRC_DIR)/CommandLineArgF.cc $(LIBS)
	mv CommandLineArgF.o $(OBJ_DIR)/CommandLineArgF.o

.PHONY : clean
clean :
	-rm -rf $(TRGT) $(OBJS)
