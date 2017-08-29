//
// Created by ajrgu on 23-Aug-17.
//

#include <memory.h>
#include <stdio.h>
#include "simulator.h"

/*
 * simulates tree from given node until inputs
 */
int simulate_tree(MultiGate* endGate, Output* output){
    switch(endGate->type){
        case OR:
            // if any of the input gates is 1, return 1
            for (int i = 0; i < endGate->inputGatesCount; ++i) {
                if(simulate_tree(endGate->inputGates[i], output) == 1){
                    return 1;
                }
            }
            // if none of the input gates are 1, return 0
            return 0;
        case AND:
            // if any of the input gates is 0, return 0
            for (int i = 0; i < endGate->inputGatesCount; ++i) {
                if(simulate_tree(endGate->inputGates[i], output) == 0){
                    return 0;
                }
            }
            // if all of the input gates are 1, return 1
            return 1;
        case XOR:
            // check to see if left and right are not equal
            if(simulate_tree(endGate->inputGates[0], output) != simulate_tree(endGate->inputGates[1], output)){
                return 1;
            } else {
                return 0;
            }
        case NOT:
            // return not the input
            return !simulate_tree(endGate->inputGates[0], output);

        case INPUT:
            return endGate->value;

        case END:
            for (int j = 0; j < endGate->inputGatesCount; ++j) {
                (output->size)++;
                output->value[j] = simulate_tree(endGate->inputGates[j], output);
            }
            break;

        default:
            printf("ERROR: INVALID GATE TYPE");

            return 0;
    }
}