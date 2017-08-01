#ifndef LOGICGATESIM_CORE_H
#define LOGICGATESIM_CORE_H

int simulate(char* inputFile);
int simulateSave(char* inputFile, char* outputFile);
void showTree(char* inputFile);
void strSplit(char* strInput, char** strOutput, char* splitToken);
void saveFile(char* outputFile);
void test_data();

#endif //LOGICGATESIM_CORE_H