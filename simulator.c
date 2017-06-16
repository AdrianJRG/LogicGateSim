//
// Created by ajrgu on 22-May-17.
//


#include <stdio.h>
#include <stdlib.h>
//#include <mem.h>
#include "simulator.h"
#include <string.h>
/*
 * Global vars
 */

Gate *root;
Gate gateStorageArray[1024];
int gateStorageCounter = 0;
int debug = 0;

/*
 * simulates tree from given node until inputs
 */
int simulate_tree(Gate *gate_node){
    switch(gate_node->gate){
        case OR:
            if(debug){printf("OR\n");}
            if(simulate_tree(gate_node->left) == 1 || simulate_tree(gate_node->right) == 1){
                return 1;
            } else {
                return 0;
            }
        case AND:
            if(debug){printf("AND\n");}
            if(simulate_tree(gate_node->left) == 1 && simulate_tree(gate_node->right) == 1){
                return 1;
            } else {
                return 0;
            }
        case XOR:
            if(debug){printf("XOR\n");}
            if(simulate_tree(gate_node->left) != simulate_tree(gate_node->right)){
                return 1;
            } else {
                return 0;
            }
        case NOT:
            if(debug){printf("NOT\n");}
            return !simulate_tree(gate_node->left);
        case INPUT_ON:
            if(debug){printf("INPUT_ON\n");}
            return 1;
        case INPUT_OFF:
            if(debug){printf("INPUT_OFF\n");}
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

int create_gates(int arraySize, char*** content, Gate *given_root){
    printf("\tNow in simulator.c create_gates\n");

    for(int i = 0; i < arraySize; i++){
        printf("%i\t%s\n", i, content[i][0]);
        //printf(&content[14][0]); printf("\n");
        if(strcmp(content[i][2], "END") == 0){
            printf("\tFound END gate at position %i\n", i);
            Gate rootGate;
            rootGate.left = NULL;
            rootGate.right = NULL;
            rootGate.name = content[i][0];
            printf("Root Gate Name: %s\n", rootGate.name);
            rootGate.gate = get_gate_type(content[i][1]);
//            gateStorageArray[gateStorageCounter] = rootGate;
//            gateStorageCounter++;
//            root = &gateStorageArray[gateStorageCounter];

            *given_root = rootGate;
            printf("\tStarting recursive_tree_build\n");
            recursive_tree_build(arraySize, content, given_root);
            printf("\tFinished recursive_tree_build\n");

            return 0;
        }
    }
    printf("\n\nError:\tDid not find END gate\n");
    return -1;
}
//
//void TEST_ARRAY(int arraySize, char*** content){
//    for(int i = 0; i < arraySize; i++){
//        printf(content[i]); printf("\n");
//    }
//}

void recursive_tree_build(int arraySize, char*** content, Gate* given_gate){
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
        if(strcmp(given_gate->name, content[i][2]) == 0 && branchesFilled == 0){
            Gate tempGateLeft;
            printf("\t\tCreating left gate for: %s named: %s\n", content[i][2], content[i][0]);
            tempGateLeft.name = content[i][0];
            tempGateLeft.gate = get_gate_type(content[i][1]);
            tempGateLeft.left = NULL;
            tempGateLeft.right = NULL;

            gateStorageArray[gateStorageCounter] = tempGateLeft;
            given_gate->left = &gateStorageArray[gateStorageCounter];

            gateStorageCounter++;

            printf("\t\tNext level of recursive_tree_build\n");
            recursive_tree_build(arraySize, content, (*given_gate).left);
            printf("\t\tReturned from previous level of recursive_tree_build\n");

            if (tempGateLeft.gate == NOT){
                return;
            }
            branchesFilled++;

        } else if(strcmp(given_gate->name, content[i][2]) == 0 && branchesFilled == 1){
            Gate tempGateRight;
            printf("\t\tCreating right gate for: %s named: %s\n", content[i][2], content[i][0]);
            tempGateRight.name = content[i][0];
            tempGateRight.gate = get_gate_type(content[i][1]);
            tempGateRight.left = NULL;
            tempGateRight.right = NULL;

            gateStorageArray[gateStorageCounter] = tempGateRight;
            given_gate->right = &gateStorageArray[gateStorageCounter];

            gateStorageCounter++;

            printf("\t\tNext level of recursive_tree_build\n");
            recursive_tree_build(arraySize, content, (*given_gate).right);
            printf("\t\tReturned from previous level of recursive_tree_build\n");

            return;
        }
    }
}

gate_type get_gate_type(char* gateString){
    printf("Gate string: %s ", gateString);
    if(strcmp(gateString, "END")) {printf("Result: END"); return END;}
    if(strcmp(gateString, "OR")) {printf("Result: OR"); return OR;}
    if(strcmp(gateString, "XOR")) {return XOR;}
    if(strcmp(gateString, "NOT")) {return NOT;}
    if(strcmp(gateString, "AND")) {return AND;}

    printf("Could not find gate_type\n");
    return END;
}

int add_input_to_tree(int* input, Gate* gate_node){
    /*
     * Go down left, add next input bit
     * Go down right, add next input bit
     */
    static int counter = 0;

    /*
    char* temp_string = (*gate_node).name;
    if(temp_string == NULL || strcmp(temp_string, "") == 0){
        printf("Its empty\n");
    }
    printf("%s\n", temp_string);
    printf("temp in add_input_to_tree\n");
    //*/

    if(gate_node->left != NULL){
        printf("\t\tGate left of %s was not NULL\n", gate_node->name);
        add_input_to_tree(input, gate_node->left);
    } else {
        printf("\t\tGate left of %s was NULL\n", gate_node->name);
        // Add input[counter] as left input, where 0 is INPUT_OFF and 1 is INPUT_ON
        // If input is too short, the remaining values will be assumed 0
        if(counter > (sizeof(input)/sizeof(int)) || input[counter] == 0){
            Gate tempGateOff;
            tempGateOff.name = "input";
            tempGateOff.gate = INPUT_OFF;
            tempGateOff.left = NULL;
            tempGateOff.right = NULL;
            gateStorageArray[gateStorageCounter] = tempGateOff;
            gate_node->left = &gateStorageArray[gateStorageCounter];
            gateStorageCounter++;
        } else {
            Gate tempGateOn;
            tempGateOn.name = "input";
            tempGateOn.gate = INPUT_ON;
            tempGateOn.left = NULL;
            tempGateOn.right = NULL;
            gateStorageArray[gateStorageCounter] = tempGateOn;
            gate_node->left = &gateStorageArray[gateStorageCounter];
            gateStorageCounter++;
        }
        counter++;
    }

    //only check on right side if not a NOT gate
    if(gate_node->gate != NOT){
       if(gate_node->right != NULL){
           printf("\t\tGate right of %s was not NULL\n", gate_node->name);
           add_input_to_tree(input, gate_node->right);
       } else {
           printf("\t\tGate right of %s was NULL\n", gate_node->name);
           if(counter > (sizeof(input)/sizeof(int)) || input[counter] == 0){
               Gate tempGateOff;
               tempGateOff.name = "input";
               tempGateOff.gate = INPUT_OFF;
               tempGateOff.left = NULL;
               tempGateOff.right = NULL;
               gateStorageArray[gateStorageCounter] = tempGateOff;
               gate_node->right = &gateStorageArray[gateStorageCounter];
               gateStorageCounter++;
           } else {
               Gate tempGateOn;
               tempGateOn.name = "input";
               tempGateOn.gate = INPUT_ON;
               tempGateOn.left = NULL;
               tempGateOn.right = NULL;
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
    if(gate_node->left->gate == INPUT_OFF ||
            gate_node->left->gate == INPUT_ON){
        gate_node->left = NULL;
    } else {
        remove_inputs_from_tree(gate_node->left);
    }

    if(gate_node->right->gate == INPUT_OFF ||
       gate_node->right->gate == INPUT_ON){
        gate_node->right = NULL;
    } else {
        remove_inputs_from_tree(gate_node->right);
    }

    // For now, memory will not be freed. May be implemented later on
//
//    if(gate_node->gate == INPUT_ON || gate_node->gate == INPUT_OFF){
//        destroy_tree(gate_node);
//    } else {
//        if(gate_node->left != NULL)
//            remove_inputs_from_tree(gate_node->left);
//        if(gate_node->right != NULL)
//            remove_inputs_from_tree(gate_node->right);
//    }
}
