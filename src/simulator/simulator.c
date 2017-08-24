//
// Created by ajrgu on 23-Aug-17.
//

#include <memory.h>
#include <stdio.h>
#include "simulator.h"

/*
 * simulates tree from given node until inputs
 */
int simulate_tree(MultiGate* endGate, Input* input, Output* output){
    switch(endGate->type){
        case OR:
            // if any of the input gates is 1, return 1
            for (int i = 0; i < endGate->inputGatesCount; ++i) {
                if(simulate_tree(endGate->inputGates[i], input, output) == 1){
                    return 1;
                }
            }
            // if none of the input gates are 1, return 0
            return 0;
        case AND:
            // if any of the input gates is 0, return 0
            for (int i = 0; i < endGate->inputGatesCount; ++i) {
                if(simulate_tree(endGate->inputGates[i], input, output) == 0){
                    return 0;
                }
            }
            // if all of the input gates are 1, return 1
            return 1;
        case XOR:
            // check to see if left and right are not equal
            if(simulate_tree(endGate->inputGates[0], input, output) != simulate_tree(endGate->inputGates[1], input, output)){
                return 1;
            } else {
                return 0;
            }
        case NOT:
            // return not the input
            return !simulate_tree(endGate->inputGates[0], input, output);

        case INPUT:
            // check which input (stored in name)
            if(strcmp(endGate->name, "1")){
                return 1;
            } else
            if (strcmp(endGate->name, "0")){
                return 0;
            }

        default:
            printf("ERROR: INVALID GATE TYPE");

            return 0;
    }
}