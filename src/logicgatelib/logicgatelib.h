//
// Created by ajrgu on 23-Aug-17.
//

#ifndef LOGICGATESIM_LOGICGATELIB_C
#define LOGICGATESIM_LOGICGATELIB_C

#define MAX_INPUT_GATES 8
#define MAX_INPUT_SIZE 24
#define MAX_OUTPUT_SIZE 24

#include <stdint.h>

// List of gates to enum
typedef enum {OR, AND, XOR, NOT, END, INPUT} gate_type;

typedef struct MultiGate {
    char* name;
    gate_type type;
    struct MultiGate* inputGates[MAX_INPUT_GATES];
    int inputGatesCount;
} MultiGate;

typedef struct Input {
    int size;
    uint8_t value[MAX_INPUT_SIZE];
} Input;

typedef struct Output {
    int size;
    uint8_t value[MAX_OUTPUT_SIZE];
} Output;

#endif //LOGICGATESIM_LOGICGATELIB_C
