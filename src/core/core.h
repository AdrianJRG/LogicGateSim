//
// Created by ajrgu on 29-Aug-17.
//

#ifndef LOGICGATESIM_CORE_H
#define LOGICGATESIM_CORE_H

#define MAX_NUMBER_OF_INPUTS 50

int loadFromFile(char* fileName, int verbose);
int saveToFile(char* fileName);
int cleanHeap(void);
int simulate(void);

#endif //LOGICGATESIM_CORE_H
