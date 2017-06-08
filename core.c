#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "IOfile.h"
#include "simulator.h"
/*
 * Things this will do:
 *
 * tell IOfile to save
 * tell IOfile to open
 * tell simulator to simulate
 * tell simulator to add gate_node (build node here or in simulator??)
 * get results from simulator
 * print results
 * save results
 */

/*
 * global vars
 */

int* sizeContent;
int* sizeInput;
char** content;
int** input;

Gate *root;

/*
 * helper functions
 */

void strSplit(char* strInput, char** strOutput)
{
	char* p = strtok(strInput, " ");
	int i = 0;
	while(p !=NULL)
	{	
		strOutput[i++] = p;
		p = strtok(NULL, " ");
	}
}

void create_tree_from_file(char* inputFile){
    readFile(inputFile, sizeContent, sizeInput, content, input);
    *root = create_gates(*sizeContent, content);
}

/*
 * End of helper functions
 */

int simulate(char* inputFile){
    create_tree_from_file(inputFile);

    //prints results from simulations
    for(int i = 0; i < *sizeInput; i++){
        add_input_to_tree(input[i], root);
        int result = simulate_tree(root);
        printf("%d\n", result);
        remove_inputs_from_tree(root);
    }
    return 0;
}

int simulateSave(char* inputFile, char* outputFile){
    simulate(inputFile);

    printf("simulateSave not implemented");

    return 999;

}

void showTree(char* inputFile){
    printf("showTree not implemented");
}

void saveFile(char* outputFile){
    printf("saveTree not implemented");
}


/*
 * temp funtions
 */

void test_data(){
    printf("Now in code.c test_data()\n");

    printf("Test data array creation\n");
    char* test_data_array[5][3] = {
            {"A", "AND", "B"},
            {"C", "AND", "B"},
            {"D", "NOT", "E"},
            {"B", "OR", "E"},
            {"E", "AND", "END"}
    };

    char* test_row1[3] = {"A", "AND", "B"};
    char* test_test[2] = {
            *test_row1,

    };

    printf("TEST_ARRAY\n");
    //TEST_ARRAY(5, test_data_array);
    printf("Calling create_gates from simulator\n");
    *root = create_gates(5, test_data_array);

    printf("Creating test input data array\n");
    int test_data_inputs[2][5] = {
            {1, 1, 1, 1, 0},
            {0, 0, 0, 1, 1}
    };

    for(int i = 0; i < 2; i++){
        add_input_to_tree(test_data_inputs[i], root);
        int result = simulate_tree(root);
        printf("%d\n", result);
        remove_inputs_from_tree(root);
    }

    printf("End of test_data()");
}