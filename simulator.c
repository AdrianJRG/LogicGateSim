//
// Created by ajrgu on 22-May-17.
//


#include <stdio.h>
#include <stdlib.h>
#include <mem.h>
#include "simulator.h"

/*
 * Global vars
 */

Gate *root;
Gate gateStorageArray[256];
int gateStorageCounter = 0;

/*
 * simulates tree from given node until inputs
 */
int simulate_tree(Gate *gate_node){
    switch(gate_node->gate){
        case OR:
            if(simulate_tree(gate_node->left) == 1 || simulate_tree(gate_node->right) == 1){
                return 1;
            } else {
                return 0;
            }
        case AND:
            if(simulate_tree(gate_node->left) == 1 && simulate_tree(gate_node->right) == 1){
                return 1;
            } else {
                return 0;
            }
        case XOR:
            if(simulate_tree(gate_node->left) != simulate_tree(gate_node->right)){
                return 1;
            } else {
                return 0;
            }
        case NOT:
            return !simulate_tree(gate_node->left);
        case INPUT_ON:
            return 1;
        case INPUT_OFF:
            return 0;
        default:
            printf("Invalid gate_type in simulate_tree(), returning 0\n");
            return 0;
    }
}

/*
 * Destroys everything below the given node, as well as the node itself
 */
void destroy_tree(Gate *gate_node){
    if(gate_node != 0){
        destroy_tree(gate_node->left);
        destroy_tree(gate_node->right);
        free(gate_node);
    }
}

/*
 * Checks if left is empty, then if right is empty
 * Places the branch to insert at the first empty node found.
 */
void insert_tree(Gate *gate_nodeToInsert, Gate *gate_nodeToAttachTo){

    if(gate_nodeToAttachTo->left == 0){
        gate_nodeToAttachTo->left = gate_nodeToInsert;
        return;
    } else if (gate_nodeToAttachTo->right == 0 && gate_nodeToAttachTo->gate != NOT){  // makes sure not gates only have 1 input
        gate_nodeToAttachTo->right = gate_nodeToInsert;
        return;
    }

    printf("More than 2 logic gates are being attached to the inputs of another(?) or more than 1 to a NOT gate \n");
    return;
}

/*
 * Searches node and all those under it for uid
 */
//Gate *search_tree(uint8_t uid, Gate *gate_node){
//
//    /*
//     * 1. Check if this gate_node is the one being searched for
//     *      if yes, return gate_node
//     * 2. Check if left gate_node is the one
//     *
//     * 3. Check if right gate_node is the one
//     */
//    if(gate_node->uid == uid){
//        return gate_node;
//    }
//
//    Gate *n = NULL; // Might need to be 0 (?), based on some c++ code
//
//    if(gate_node->left != 0)
//        n = search_tree(uid, gate_node->left);
//    if(n != NULL)
//        return n;
//
//    if(gate_node->right != 0)
//        n = search_tree(uid, gate_node->right);
//    if(n != NULL)
//        return n;
//
//    return NULL;
//}

/*
 * Counts total nodes in tree
 */
int count_tree(Gate *gate_node){
    if(gate_node != 0){
        // itself, sum of left, sum of right
        return 1 + count_tree(gate_node->left) + count_tree(gate_node->right);
    } else {
        return 0;
    }
}

Gate create_gates(int arraySize, char** content){
    for(int i = 0; i < arraySize; i++){
        if(strcmp(&content[i][2], "END") == 0){
            Gate rootGate;
            rootGate.name = &content[i][0];
            rootGate.gate = get_gate_type(&content[i][1]);

            gateStorageArray[gateStorageCounter] = rootGate;
            gateStorageCounter++;
            *root = gateStorageArray[gateStorageCounter];

            recursive_tree_build(arraySize, content, *root);

            return *root;
        }
    }
    return NULL;
}

void recursive_tree_build(int arraySize, char** content, Gate gate){
    int branchesFilled = 0;
    for(int i = 0; i < arraySize; i++){
        /*
         * If the output of a gate is equal to current gate
         *      Add that gate to the left
         *      Recursive search on left branch
         * If it is not a NOT gate
         *      Search for another and add to right
         *      Recursive search on right branch
         */
        if(strcmp(gate.name, &content[i][2]) == 0 && branchesFilled == 0){
            Gate tempGateLeft;
            tempGateLeft.name = &content[i][0];
            tempGateLeft.gate = get_gate_type(&content[i][1]);

            gateStorageArray[gateStorageCounter] = tempGateLeft;
            gate.left = &gateStorageArray[gateStorageCounter];

            gateStorageCounter++;

            recursive_tree_build(arraySize, content, *gate.left);

            if (tempGateLeft.gate != NOT){
                return;
            }
            branchesFilled++;

        } else if(strcmp(gate.name, &content[i][2]) == 0 && branchesFilled == 1){
            Gate tempGateRight;
            tempGateRight.name = &content[i][0];
            tempGateRight.gate = get_gate_type(&content[i][1]);

            gateStorageArray[gateStorageCounter] = tempGateRight;
            gate.right = &gateStorageArray[gateStorageCounter];

            gateStorageCounter++;

            recursive_tree_build(arraySize, content, *gate.right);

            return;
        }
    }
}

gate_type get_gate_type(char* gateString){
    if(strcmp(gateString, "END")) return END;
    if(strcmp(gateString, "OR")) return OR;
    if(strcmp(gateString, "XOR")) return XOR;
    if(strcmp(gateString, "NOT")) return NOT;
    if(strcmp(gateString, "AND")) return AND;

    return NULL;
}

int add_input_to_tree(int* input, Gate* gate_node){
    /*
     * Go down left, add next input bit
     * Go down right, add next input bit
     */
    static int counter = 0;

    if(gate_node->left != NULL){
        add_input_to_tree(input, gate_node->left);
    } else {
        // Add input[counter] as left input, where 0 is INPUT_OFF and 1 is INPUT_ON
        // If input is too short, the remaining values will be assumed 0
        if(counter > (sizeof(input)/sizeof(int)) || input[counter] == 0){
            Gate tempGateOff;
            tempGateOff.name = "input" + counter;
            tempGateOff.gate = INPUT_OFF;
            gateStorageArray[gateStorageCounter] = tempGateOff;
            gate_node->left = &gateStorageArray[gateStorageCounter];
            gateStorageCounter++;
        } else {
            Gate tempGateOn;
            tempGateOn.name = "input" + counter;
            tempGateOn.gate = INPUT_ON;
            gateStorageArray[gateStorageCounter] = tempGateOn;
            gate_node->left = &gateStorageArray[gateStorageCounter];
            gateStorageCounter++;
        }
        counter++;
    }

    //only check on right side if not a NOT gate
    if(gate_node->gate != NOT){
       if(gate_node->right != NULL){
           add_input_to_tree(input, gate_node->right);
       } else {
           if(counter > (sizeof(input)/sizeof(int)) || input[counter] == 0){
               Gate tempGateOff;
               tempGateOff.name = "input" + counter;
               tempGateOff.gate = INPUT_OFF;
               gateStorageArray[gateStorageCounter] = tempGateOff;
               gate_node->right = &gateStorageArray[gateStorageCounter];
               gateStorageCounter++;
           } else {
               Gate tempGateOn;
               tempGateOn.name = "input" + counter;
               tempGateOn.gate = INPUT_ON;
               gateStorageArray[gateStorageCounter] = tempGateOn;
               gate_node->right = &gateStorageArray[gateStorageCounter];
               gateStorageCounter++;
           }
           counter++;
       }
    }

    return counter;
}

void remove_inputs_from_tree(Gate* gate_node){
    if(gate_node->gate == INPUT_ON || gate_node->gate == INPUT_OFF){
        destroy_tree(gate_node);
    } else {
        if(gate_node->left != NULL)
            remove_inputs_from_tree(gate_node->left);
        if(gate_node->right != NULL)
            remove_inputs_from_tree(gate_node->right);
    }
}
