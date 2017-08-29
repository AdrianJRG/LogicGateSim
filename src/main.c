#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core/core.h"
#include "logicgatelib/logicgatelib.h"

int main(int argc, char** argv) {
	int isRunning = 1;
	char readLine[100];
	char* arguments[3];
	char* file = "./testinput.txt";
	if(argc == 2)
	{
		//if argc == 2 the second argument is an input file
		//simulate(argv[1]);
		
	}
	 else if(argc == 3)
	{	
		//if argc == 3 the second argument is input, third output file
		//simulateSave(argv[1], argv[2]);
	}
	while(isRunning)
	{
		fgets(readLine, sizeof(readLine), stdin);
		strtok(readLine, "\n");
        int sizeofsplit;
		strSplit(readLine, arguments, &sizeofsplit, " ");
		/*
		options for cli
		show [inputFile]
		run (optional)[inputFile]
		open [inputFile] 
		save [outputFile]
		
		quit
		*/
		if(strcmp(arguments[0], "show")==0)
		{
			//showTree(arguments[1]);
		}
		else if(strcmp(arguments[0], "run")==0)
		{
			if(loadFromFile(arguments[1]) != 0){printf("Error occurred\n"); return -1;}
			simulate();
		}
		else if(strcmp(arguments[0], "quit")==0)
		{
			isRunning = 0;
		}
		else
		{
			printf("Didn't recognize the input\n");
		}
	}
	return 0;
}

