//
// Created by ajrgu on 23-Aug-17.
//

#include "../logicgatelib/logicgatelib.h"

#ifndef LOGICGATESIM_IOFILE_H
#define LOGICGATESIM_IOFILE_H

int readFile(char* fileName, MultiGate* endGate, Input** input);
int writeFile(char* fileName, MultiGate* endGate, Output** output);

//temp
void strSplit(char* strInput, char** strOutput, int* outputSize, char* splitToken);
int lineToGate(char* line, MultiGate* gate);

#endif //LOGICGATESIM_IOFILE_H
