#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/core.h"
#include "logicgatelib/logicgatelib.h"

int help() {
	printf("Usage: logicgatesim [--help] [-i filename] [-o filename]\n");
	printf("\t--help        : show this message\n");
	printf("\t-i <filename> : file name of input file\n");
	printf("\t-o <filename> : file name of output file\n");
	printf("\t-v            : enable verbose output\n");
	return 1;
}

int help_cli(){
	printf("Usage:\n");
	printf("\thelp\t\t\tshow this message again\n");
	printf("\trun [inputFile]\t\topen file and simulate\n");
	printf("\topen [inputFile]\topen the file\n");
	printf("\tsimulate\t\trun the simulation\n");
	printf("\tsave [outputFile]\tsave to file\n");
	printf("\tquit\n");
	return 0;
}

int credit(){
    printf("LogicGateSim version 1.0\n");
    printf("\nSee https://github.com/adrianjrg/LogicGateSim for more information\n");
    printf("\n\tMade by: Adrian Guillot, Bas Bonthuis, Roy Scheepers\n");
    return 0;
}

int main(int argc, char** argv) {
	int isRunning = 1;
	char readLine[100];
	char* arguments[3];

	// If command line arguments are given, execute those instead of opening a cli
	if(argc > 1)
	{
		char* inputFile = NULL;
		char* outputFile = NULL;
        int verbose = 0;

		int i;
		for (i = 1; i < argc; i++) {
			if (strcmp("-v", argv[i]) == 0) {
				verbose = 1;
			}
			if (strcmp("--version", argv[i]) == 0) {
				return credit();
			}
			if (strcmp("--help", argv[i]) == 0) {
				return help();
			}
			if (strcmp("-i", argv[i]) == 0) {
                if(i + 1 > argc - 1){ return help();}
				inputFile = argv[++i];
                continue;
            }
            if (strcmp("-o", argv[i]) == 0) {
                if(i + 1 > argc - 1){ return help();}
                outputFile = argv[++i];
				continue;
			}
            printf("Unrecognized input, use \"--help\" for more information\n");
			return 1;
		}

		if(inputFile == NULL){ printf("Missing input file\n"); return help();}

		//Check if file is loaded properly
		if(loadFromFile(inputFile, verbose) != 0) return 1;
		// simulate it
		simulate();

		if(outputFile != NULL){
			saveToFile(outputFile);
		}

        cleanHeap();
		return 0;
	}

	// no arguments given, so cli is started
	printf("LogicGateSim 1.0\n");
	printf("\tType \"help\" for usage information\n");
	int fileOpen = 0;
	int simulated = 0;
	while(isRunning)
	{
		fgets(readLine, sizeof(readLine), stdin);
		strtok(readLine, "\n");
        int sizeOfSplit;
		strSplit(readLine, arguments, &sizeOfSplit, " ");
		if(strcmp(arguments[0], "help") == 0)
		{
			help_cli();
		}
		else if(strcmp(arguments[0], "run")==0)
		{
			if(loadFromFile(arguments[1], 0) != 0){printf("Error occurred\n"); return -1;}
			fileOpen = 1;
			simulate();
			simulated = 1;
		}
		else if(strcmp(arguments[0], "open")==0)
		{
			if(loadFromFile(arguments[1], 0) != 0){printf("Error occurred\n"); return -1;}
			fileOpen = 1;
		}
		else if(strcmp(arguments[0], "simulate")==0)
		{
			if(fileOpen != 0){
				simulate();
				simulated = 1;
			}
		}
		else if(strcmp(arguments[0], "save")==0)
		{
			if(simulated != 0){
				saveToFile(arguments[1]);
			}
		}
		else if(strcmp(arguments[0], "quit")==0)
		{
            cleanHeap();
			isRunning = 0;
		}
		else
		{
			printf("Didn't recognize the input. Type \"help\" for more valid commands\n");
		}
	}
	return 0;
}

