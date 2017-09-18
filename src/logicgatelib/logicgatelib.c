//
// Created by ajrgu on 23-Aug-17.
//

#include "logicgatelib.h"

void strSplit(char* strInput, char** strOutput, int* outputSize, char* splitToken){
    char* p = strtok(strInput, splitToken);
    int i = 0;
    while(p !=NULL)
    {
        strOutput[i++] = p;
        p = strtok(NULL, splitToken);
    }

    *outputSize = i;
}