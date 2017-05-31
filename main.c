#include <stdio.h>

int main(int argc, char** argv) {
	int isRunning = 1;
	char readLine[100];
	if(argc == 2)
	{
		//if argc == 2 the second argument is an input file
		// calcResult(inputFile);
		
	}
	 else if(argc == 3)
	{	
		//if argc == 3 the second argument is input, third output file
		// calcResult(inputFile, outputFile);
	}
	while(isRunning)
	{
		fget(readLine, sizeof(readLine), stdin);
		if(strcmp(readLine, "quit"))
		{
			isRunning = 0;
		}
	}
	return 0;
}
