//
// Created by ajrgu on 22-May-17.
//

#ifndef LOGICGATESIM_SIMULATOR_H
#define LOGICGATESIM_SIMULATOR_H

#include <stdint.h>

/* Enum used to define gate type */
typedef enum {OR, AND, XOR, NOT, END, INPUT_ON, INPUT_OFF} gate_type;

/* Simulator will be stored in a binary tree */
typedef struct GateTag{
    uint8_t uid;
    gate_type gate;
    Gate *left;
    Gate *right;
} Gate;


#endif //LOGICGATESIM_SIMULATOR_H
