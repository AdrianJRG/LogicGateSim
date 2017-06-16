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
    //uint8_t uid;
    char* name;
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

int create_gates(int arraySize, char*** content, Gate* given_root);
void recursive_tree_build(int arraySize, char*** content, Gate* given_gate);
gate_type get_gate_type(char* gateString);
int add_input_to_tree(int* input, Gate* gate_node);
void remove_inputs_from_tree(Gate* gate_node);
void TEST_ARRAY(int arraySize, char*** constant);

#endif //LOGICGATESIM_SIMULATOR_H
