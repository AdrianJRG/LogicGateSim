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
    struct GateTag *left;
    struct GateTag *right;
} Gate;

/* Function prototypes */
int simulate_tree(Gate *gate_node);
void destroy_tree(Gate *gate_node);
void insert_tree(Gate *gate_nodeToInsert, Gate *gate_nodeToAttachTo);
Gate *search_tree(uint8_t uid, Gate *gate_node);
int count_tree(Gate *gate_node);


#endif //LOGICGATESIM_SIMULATOR_H
