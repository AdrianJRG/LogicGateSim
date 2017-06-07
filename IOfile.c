#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "core.h" 

int readFile(char* fileName,int* arraySizeContent,int* arraySizeInput, char** content, int** input)
{
	FILE *f;
	f = fopen(fileName, "r");
	char c = 0;
	int i = 0;
	int y = 0;
	int isInput = 0;
	char line[256];
	if(f ==NULL)
	{
		printf("Can't open file.");
	}
	else while(!feof(f))
	{
		c = fgetc(f);
		if(c != '\n' || !isInput)
		{
			memset(line, 0, sizeof(line));
			char* splitLine[3];
			ungetc(c,f);
			fscanf(f, "%[^\n]",line);
			strSplit(line, splitLine);
			content[i][0] = *splitLine[0];
			content[i][1] = *splitLine[1];
			content[i++][2] = *splitLine[2];
		} else
		{
			isInput = 1;
			c = fgetc(f);
			if(c == '1' || c == '0')
			{
				memset(line, 0, sizeof(line));
				ungetc(c, f);
				fscanf(f, "%[^\n]",line);
				for(int j = 0; j <i+1;j++)
				{
					input[y][j] = line[j];
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

