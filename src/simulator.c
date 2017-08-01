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
    int temp_left;
    int temp_right;
    if(debug){printf("%s\t", gate_node->name);}
    switch(gate_node->gate){
        case OR:
            if(debug){printf("OR\n");}
            temp_left = simulate_tree(gate_node->left);
            temp_right = simulate_tree(gate_node->right);
            if(temp_left == 1 || temp_right == 1){
                return 1;
            } else {
                return 0;
            }
        case AND:
            if(debug){printf("AND\n");}
            temp_left = simulate_tree(gate_node->left);
            temp_right = simulate_tree(gate_node->right);
            if(temp_left == 1 && temp_right == 1){
                return 1;
            } else {
                return 0;
            }
        case XOR:
            if(debug){printf("XOR\n");}
            temp_left = simulate_tree(gate_node->left);
            temp_right = simulate_tree(gate_node->right);
            if(temp_left != temp_right){
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
        case ERROR:
            printf("ERROR: Enum type ERROR");
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

    // clean out all previous gates and reset counter
    memset(gateStorageArray, 0, sizeof(gateStorageArray));
    gateStorageCounter = 0;

    if (debug) {printf("\tNow in simulator.c create_gates\n");}

    for(int i = 0; i < arraySize; i++){
        if (debug) {printf("%i\t%s\n", i, content[i][0]);}
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
            if (debug) {printf("\tStarting recursive_tree_build\n");}
            recursive_tree_build(arraySize, content, given_root);
            if (debug) {printf("\tFinished recursive_tree_build\n");}

            return 0;
        }
    }
    printf("\n\nError:\tDid not find END gate\n");
    return -1;
}

void recursive_tree_build(int arraySize, char*** content, Gate* given_gate){
    int branchesFilled = 0;
    for(int i = 0; i < arraySize; i++){
        /*
         * If the output of a gate is equal to current gate
         *      Check if gate exists
         *          Yes -> Add gate to left
         *          No -> Create gate, add to left
         *      Recursive search on left branch
         * If it is not a NOT gate
         *      Search for another output equal to current gate
         *      Check if gate exists
         *          Yes -> Add to right
         *          No -> Create gate, add to right
         *      Recursive search on right branch
         */
        if(strcmp(given_gate->name, content[i][2]) == 0 && branchesFilled == 0){
            int index_gate_search = -1;
            get_gate_if_exists(content[i][0], &index_gate_search);
            // Check if gate was found, index != 0
            if (index_gate_search == -1) {
                Gate tempGateLeft;
                if (debug) { printf("\t\tCreating left gate for: %s named: %s\n", content[i][2], content[i][0]); }
                tempGateLeft.name = content[i][0];
                tempGateLeft.gate = get_gate_type(content[i][1]);
                tempGateLeft.left = NULL;
                tempGateLeft.right = NULL;

                gateStorageArray[gateStorageCounter] = tempGateLeft;
                given_gate->left = &gateStorageArray[gateStorageCounter];

                gateStorageCounter++;

                // recursive build does not need to run if gate already exists!
                if (debug) {printf("\t\tNext level of recursive_tree_build\n");}
                recursive_tree_build(arraySize, content, (*given_gate).left);
                if (debug) {printf("\t\tReturned from previous level of recursive_tree_build\n");}

            } else {
                // add the existing gate as input
                given_gate->left = &gateStorageArray[index_gate_search];
            }

            if ((*given_gate).gate == NOT){
                return;
            }
            branchesFilled++;

        } else if(strcmp(given_gate->name, content[i][2]) == 0 && branchesFilled == 1){
            int index_gate_search = -1;
            get_gate_if_exists(content[i][0], &index_gate_search);
            // Check if gate was found, index != 0
            if (index_gate_search == -1) {
                Gate tempGateRight;
                if (debug) { printf("\t\tCreating right gate for: %s named: %s\n", content[i][2], content[i][0]); }
                tempGateRight.name = content[i][0];
                tempGateRight.gate = get_gate_type(content[i][1]);
                tempGateRight.left = NULL;
                tempGateRight.right = NULL;

                gateStorageArray[gateStorageCounter] = tempGateRight;
                given_gate->right = &gateStorageArray[gateStorageCounter];

                gateStorageCounter++;

                // recursive build does not need to run if gate already exists!
                if (debug) {printf("\t\tNext level of recursive_tree_build\n");}
                recursive_tree_build(arraySize, content, (*given_gate).right);
                if (debug) {printf("\t\tReturned from previous level of recursive_tree_build\n");}

            } else {
                // add the existing gate as input
                given_gate->right = &gateStorageArray[index_gate_search];
            }


            return;
        }
    }
}

gate_type get_gate_type(char* gateString){
    if (debug) {printf("Gate string: %s ", gateString);}
    if(strcmp(gateString, "END") == 0) {if (debug) {printf("Result: END\n");} return END;}
    if(strcmp(gateString, "OR") == 0) {if (debug) {printf("Result: OR\n");} return OR;}
    if(strcmp(gateString, "XOR") == 0) {if (debug) {printf("Result: XOR\n");} return XOR;}
    if(strcmp(gateString, "NOT") == 0) {if (debug) {printf("Result: NOT\n");} return NOT;}
    if(strcmp(gateString, "AND") == 0) {if (debug) {printf("Result: AND\n");} return AND;}

    printf("Could not find gate_type\n");
    return ERROR;
}

int add_input_to_tree(int* input, int size_of_array, Gate* gate_node) {
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
    if (debug) {
        printf("Start of array print\n");
        for (int i = 0; i < size_of_array; i++) {
            printf("%i ", input[i]);
        }
        printf("End of array print\n");
    }

    if(gate_node->left != NULL){
        if (debug) {printf("\t\tGate left of %s was not NULL\n", gate_node->name);}
        add_input_to_tree(input, size_of_array, gate_node->left);
    } else {
        if (debug) {printf("\t\tGate left of %s was NULL\n", gate_node->name);}
        // Add input[counter] as left input, where 0 is INPUT_OFF and 1 is INPUT_ON
        // If input is too short, the remaining values will be assumed 0
        if(counter < size_of_array && input[counter] == 0){
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
           if (debug) {printf("\t\tGate right of %s was not NULL\n", gate_node->name);}
           add_input_to_tree(input, size_of_array, gate_node->right);
       } else {
           if (debug) {printf("\t\tGate right of %s was NULL\n", gate_node->name);}
           if(counter < size_of_array && input[counter] == 0){
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
    /*
     * 1. Check if left is not NULL
     * 2. Check if left is input
     * 2.T Set to NULL
     * 2.F Repeat for left
     */
    if(gate_node->left != NULL) {
        if (gate_node->left->gate == INPUT_OFF ||
            gate_node->left->gate == INPUT_ON) {
            gate_node->left = NULL;
        } else {
            remove_inputs_from_tree(gate_node->left);
        }
    }

    // Same as for the left.
    if(gate_node->right != NULL) {
        if (gate_node->right->gate == INPUT_OFF ||
            gate_node->right->gate == INPUT_ON) {
            gate_node->right = NULL;
        } else {
            remove_inputs_from_tree(gate_node->right);
        }
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

// IMPORTANT NOTE: Gates with the same name MUST have the same gate type!!!
void get_gate_if_exists(char* name_of_gate, int* index){
    for(int i = 0; i < gateStorageCounter; i++){
        if (debug) {printf("Comparing\t%s\t%s\t", gateStorageArray[i].name, name_of_gate);}
        if(strcmp(gateStorageArray[i].name, name_of_gate) == 0){
            *index = i;
            if (debug) {printf("\tTrue\n");}
            return;
        }
        if (debug) {printf("\tFalse\n");}
    }
    return;
}

void freeGates(Gate* given_gate){
    memset(gateStorageArray, 0, sizeof(Gate)*1024);
    gateStorageCounter = 0;
    given_gate = NULL;
}
