#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

void strSplit(char* strInput, char* strOutput[2]);
int main(int argc, char** argv) {
	int isRunning = 1;
	char readLine[100];
	char* arguments[2];
	if(argc == 2)
	{
		//if argc == 2 the second argument is an input file
		simulate(argv[1]);
		
	}
	 else if(argc == 3)
	{	
		//if argc == 3 the second argument is input, third output file
		simulateSave(argv[1], argv[2]);
	}
	while(isRunning)
	{
		fgets(readLine, sizeof(readLine), stdin);
		strtok(readLine, "\n");
		strSplit(readLine,arguments);
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
			showTree(arguments[1]);
		}
		else if(strcmp(arguments[0], "run")==0)
		{
			simulate(arguments[1]);
		}
		else if(strcmp(arguments[0], "save")==0)
		{
			saveFile(arguments[1]);
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

void strSplit(char* strInput, char* strOutput[2])
{
	char* p = strtok(strInput, " ");
	int i = 0;
	while(p != NULL)
	{
		strOutput[i++] = p;
		p = strtok(NULL, " ");
	}	
}
