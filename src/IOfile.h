#ifndef LOGICGATESIM_IOFILE_H
#define LOGICGATESIM_IOFILE_H

int readFile(char* fileName,int* arraySizeContent, int* arraySizeInput, char** content, int** input);

int readFromFile(char* fileName,int* arraySizeContent, int* arraySizeInput, char*** content, int** input);
int resetArrays(int* arraySizeContent,int* arraySizeInput, char*** content, int** input);
int lineToContentArray(char* line, int* arraySizeContent, char*** content);
int lineToInputArray(char* line, int* arraySizeInput, int** input);

int writeToFile(char* fileName, int numberOfLines, char** linesToWrite);

#endif