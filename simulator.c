//
// Created by ajrgu on 22-May-17.
//


#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

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
            break;
        case AND:
            if(simulate_tree(gate_node->left) == 1 && simulate_tree(gate_node->right) == 1){
                return 1;
            } else {
                return 0;
            }
            break;
        case XOR:
            if(simulate_tree(gate_node->left) != simulate_tree(gate_node->right)){
                return 1;
            } else {
                return 0;
            }
            break;
        case NOT:
            return !simulate_tree(gate_node->left);
            break;
        case INPUT_ON:
            return 1;
            break;
        case INPUT_OFF:
            return 0;
            break;
        default:
            printf("Something broke in simulate_tree()\n");
            break;
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
Gate *search_tree(uint8_t uid, Gate *gate_node){

    /*
     * 1. Check if this gate_node is the one being searched for
     *      if yes, return gate_node
     * 2. Check if left gate_node is the one
     *
     * 3. Check if right gate_node is the one
     */
    if(gate_node->uid == uid){
        return gate_node;
    }

    Gate *n = NULL; // Might need to be 0 (?), based on some c++ code

    if(gate_node->left != 0)
        n = search_tree(uid, gate_node->left);
    if(n != NULL)
        return n;

    if(gate_node->right != 0)
        n = search_tree(uid, gate_node->right);
    if(n != NULL)
        return n;

    return NULL;
}

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
