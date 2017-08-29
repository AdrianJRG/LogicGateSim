//
// Created by ajrgu on 23-Aug-17.
//

#include "../logicgatelib/logicgatelib.h"

#ifndef LOGICGATESIM_IOFILE_H
#define LOGICGATESIM_IOFILE_H

int readFile(char* fileName, MultiGate* endGate, Input** input, int* inputCount);
int writeFile(char* fileName, MultiGate* endGate, Output** output);

typedef enum {NONE, GATES, CONNECTIONS, INPUTS} INPUT_SECTIONS;

//temp
int lineToGate(char* line, MultiGate* gate);
int stringToGateType(char* gateTypeString);

#endif //LOGICGATESIM_IOFILE_H
