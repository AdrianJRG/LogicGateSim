//
// Created by ajrgu on 23-Aug-17.
//

#include "../logicgatelib/logicgatelib.h"

#ifndef LOGICGATESIM_IOFILE_H
#define LOGICGATESIM_IOFILE_H

int readFile(char* fileName, MultiGate* endGate, Input** input);
int writeFile(char* fileName, MultiGate* endGate, Output** output);

#endif //LOGICGATESIM_IOFILE_H
