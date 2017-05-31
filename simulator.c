//
// Created by ajrgu on 22-May-17.
//


#include <stdio.h>
#include <stdlib.h>
#include "simulator.h"

/*
 * simulates tree from given node until inputs
 */
int simulate_tree(Gate *leaf){
    switch(leaf->gate){
        case OR:
            if(simulate_tree(leaf->left) == 1 || simulate_tree(leaf->right) == 1){
                return 1;
            } else {
                return 0;
            }
            break;
        case AND:
            if(simulate_tree(leaf->left) == 1 && simulate_tree(leaf->right) == 1){
                return 1;
            } else {
                return 0;
            }
            break;
        case XOR:
            if(simulate_tree(leaf->left) != simulate_tree(leaf->right)){
                return 1;
            } else {
                return 0;
            }
            break;
        case NOT:
            return !simulate_tree(leaf->left);
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
void destroy_tree(Gate *leaf){
    if(leaf != 0){
        destroy_tree(leaf->left);
        destroy_tree(leaf->right);
        free(leaf);
    }
}

/*
 * Checks if left is empty, then if right is empty
 * Places the branch to insert at the first empty node found.
 */
void insert_tree(Gate *leafToInsert, Gate *leafToAttachTo){

    if(leafToAttachTo->left == 0){
        leafToAttachTo->left = leafToInsert;
        return;
    } else if (leafToAttachTo->right == 0 && leafToAttachTo->gate != NOT){  // makes sure not gates only have 1 input
        leafToAttachTo->right = leafToInsert;
        return;
    }

    printf("More than 2 logic gates are being attached to the inputs of another(?) or more than 1 to a NOT gate \n");
    return;
}

/*
 * Searches node and all those under it for uid
 */
Gate *search_tree(uint8_t uid, Gate *leaf){

    /*
     * 1. Check if this leaf is the one being searched for
     *      if yes, return leaf
     * 2. Check if left leaf is the one
     *
     * 3. Check if right leaf is the one
     */
    if(leaf->uid == uid){
        return leaf;
    }

    Gate *n = NULL; // Might need to be 0 (?), based on some c++ code

    if(leaf->left != 0)
        n = search_tree(uid, leaf->left);
    if(n != NULL)
        return n;

    if(leaf->right != 0)
        n = search_tree(uid, leaf->right);
    if(n != NULL)
        return n;

    return NULL;
}

/*
 * Counts total nodes in tree
 */
int count_tree(Gate *leaf){
    if(leaf != 0){
        // itself, sum of left, sum of right
        return 1 + count_tree(leaf->left) + count_tree(leaf->right);
    } else {
        return 0;
    }
}
