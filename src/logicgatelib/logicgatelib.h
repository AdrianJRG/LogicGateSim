//
// Created by ajrgu on 23-Aug-17.
//

#ifndef LOGICGATESIM_LOGICGATELIB_C
#define LOGICGATESIM_LOGICGATELIB_C

#define MAX_INPUT_GATES 8
#define MAX_INPUT_SIZE 24
#define MAX_OUTPUT_SIZE 24

#include <stdint.h>
#include <stdio.h>
#include <memory.h>
#include <stdlib.h>

// List of gates to enum
typedef enum {OR, AND, XOR, NOT, END, INPUT} gate_type;

typedef struct MultiGate {
    char* name;
    gate_type type;
    struct MultiGate* inputGates[MAX_INPUT_GATES];
    int inputGatesCount;
    int value; // used only by input gates
} MultiGate;

typedef struct Input {
    int size;
    uint8_t value[MAX_INPUT_SIZE];
} Input;

typedef struct Output {
    int size;
    int value[MAX_OUTPUT_SIZE];
} Output;

/*
 * Splits a string into an array of strings
 */
void strSplit(char* strInput, char** strOutput, int* outputSize, char* splitToken);

#endif //LOGICGATESIM_LOGICGATELIB_C
