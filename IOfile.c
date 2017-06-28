#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "core.h"

#define _DEBUG 0

//this one is unused!!! use readFromFile instead!!
int readFile(char* fileName,int* arraySizeContent,int* arraySizeInput, char** content, int** input)
{
	//First the file is opened
	FILE *f;
	if(_DEBUG) {printf("test-1");}
	f = fopen(fileName, "r");
	char c = 0;
	int i = 0;
	int y = 0;
	if(_DEBUG) {printf("test0");}
	int isInput = 0;
	char line[256];
	if(f ==NULL)
	{
		printf("Can't open file.\n");
	}
	else while(!feof(f))
	{
		c = fgetc(f);
		if(c != '\n' && !isInput)
		{
			if(_DEBUG) {printf("test 1");}
			memset(line, 0, sizeof(line));
			char* splitLine[3];
			ungetc(c,f);
			fscanf(f, "%[^\n]",line);
			strSplit(line, splitLine, " ");
			strcpy(splitLine[0], content[i]);
			strcpy(splitLine[1], content[i]+1);
			strcpy(splitLine[2], content[i]+2);
		} else
		{
			if(_DEBUG) {printf("test 2");}
			isInput = 1;
			c = fgetc(f);
			if(c == '1' || c == '0')
			{
				memset(line, 0, sizeof(line));
				ungetc(c, f);
				fscanf(f, "%[^\n]",line);
				for(int j = 0; j <i+1;j++)
				{
					input[y][j] =atoi(&line[j]);
				}
				y++;
			}
		}

	}	
	*arraySizeInput = y;
	*arraySizeContent = i;	
	//fread(content, sizeof(char),*arraySize, f);
	fclose(f);
	return 0;
}

//this is the one that is used
int readFromFile(char* fileName,int* arraySizeContent, int* arraySizeInput, char*** content, int** input){
	// first we reset the arrays
	resetArrays(arraySizeContent, arraySizeInput, content, input);

	// next we open the file
	FILE *f;
	f = fopen(fileName, "r");
	if(f == NULL){ printf("Can't open file: %s\n", fileName); return -1; }

	int lineLength = 80;
	char currentLine[lineLength]; // where current line is stored before being manipulated

	// now we will parse the gates to put into ***content
	if (fgets(currentLine, lineLength, f) == NULL) {printf("error reading line from file at %i\n", __LINE__); return -1;}
	while(strcmp(currentLine, "INPUT\n") != 0 && strcmp(currentLine, "\n") != 0){
		lineToContentArray(currentLine, arraySizeContent, content);
        if (fgets(currentLine, lineLength, f) == NULL) {printf("error reading line from file at %i\n", __LINE__); return -1;}
	}

    // make sure we get to INPUT
    while (strcmp(currentLine, "INPUT\n") != 0 && strcmp(currentLine, "INPUT") != 0){
        if (fgets(currentLine, lineLength, f) == NULL) {printf("error reading line from file at %i\n", __LINE__); return -1;}
    }

	// now we will parse the inputs to put into **inputs
    if (fgets(currentLine, lineLength, f) == NULL) {printf("error reading line from file at %i\n", __LINE__); return -1;}
	while(strcmp(currentLine, "\n") != 0 && !feof(f)){
		lineToInputArray(currentLine, arraySizeInput, input);
        if (fgets(currentLine, lineLength, f) == NULL) {printf("error reading line from file at %i\n", __LINE__); return -1;}
	}

	//finally we close the file
	fclose(f);

	return 0;
}

// start helper functions for readFromFile
int resetArrays(int* arraySizeContent,int* arraySizeInput, char*** content, int** input){
	//Reset the content array
	for(int i = 0; i < *arraySizeContent; i++){
		// free name, type, output of each gate
//		for (int j = 0; j < 3; ++j) {
//			free(content[i][j]);
//		}
        free(content[i]);
	}
	*arraySizeContent = 0;

	//Reset the input array

    //  disabled because of SIGTRAP issue!
    /*
	for(int i = 0; i < *arraySizeInput; i++){
		free(input[i]);
	}
    //*/
	*arraySizeInput = 0;

	return 0;
}

int lineToContentArray(char* line, int* arraySizeContent, char*** content){
	// expected line format: "A AND B"
    strtok(line, "\n"); //remove trailing newline if it exists

    // split the string
	char* gateSplitLine[3];
	strSplit(line, gateSplitLine, " ");

	// we make sure to save these on the heap, since stack will be unavailable
    int a = sizeof(gateSplitLine[0]);
	char* name = (char*)malloc(sizeof(char)*30);
	strcpy(name, gateSplitLine[0]);
	char* type = name + (sizeof(char)*10);
	strcpy(type, gateSplitLine[1]);
	char* output = type + (sizeof(char)*10);
	strcpy(output, gateSplitLine[2]);

	// next we create a pointer array to the three parts, also on the heap
	char** gate = (char**)malloc(sizeof(char*)*3);
	gate[0] = name;
	gate[1] = type;
	gate[2] = output;

    if(_DEBUG){printf("Gate print: %s, %s, %s\n", gate[0], gate[1], gate[2]);}
	// finally we put a pointer to the gate in content and increment the size of content
	content[(*arraySizeContent)++] = gate;

    if(_DEBUG) {
        int b = *arraySizeContent - 1;
        printf("content print: %s", content[b][0]);
        printf(" %s", content[b][1]);
        printf(" %s\n", content[b][2]);
    }

	return 0;
}

int lineToInputArray(char* line, int* arraySizeInput, int** input){
	// expected line format: "1010"
    strtok(line, "\n"); //remove trailing newline if it exists

	int maxSizeLine = 80;

	// temporary storage for input array
	int intArrayIndex = 0;
	int intArray[maxSizeLine];

	for (int i = 0; i < maxSizeLine; ++i) {
		char c = line[i];

        if(c == '\0'){i = maxSizeLine;}

		if(c == '0'){
			intArray[intArrayIndex++] = 0;
		} else if (c == '1'){
			intArray[intArrayIndex++] = 1;
		} else {
			printf("Unexpected character %c on input line, ignoring\n", c);
		}
	}

	// the int array is moved to heap and a sentinel (-1) is added to the end
	int* intArrayOnHeap = (int*)malloc(sizeof(int)*intArrayIndex + 1);
	for (int j = 0; j < intArrayIndex; ++j) {
		intArrayOnHeap[j] = intArray[j];
	}
    intArrayOnHeap[intArrayIndex] = -1;

	// finally we put a pointer to the int array in input and increment size of input
	input[(*arraySizeInput)++] = intArrayOnHeap;

	return 0;
}
// end helper functions for readFromFile

//writes results of each simulation to file
int writeToFile(char* fileName, int numberOfLines, char** linesToWrite){
    FILE* f;
    //w+b truncates the file to 0 bytes, this overwrites everything if a file already exists
    f = fopen(fileName, "w+b");
    if(f == NULL) {
        return -1;
    }

    fprintf(f, "Results of simulation:\n");
    for (int i = 0; i < numberOfLines; ++i) {
        fprintf(f, "%s\n", linesToWrite[i]);
    }
    fprintf(f, "\n");

    fclose(f);

    return 0;
}
