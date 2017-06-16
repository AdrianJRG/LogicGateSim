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
Gate test_root;

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
    //*root = create_gates(*sizeContent, content);
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
    char* test_data_array[][3] = { // This one does not work
            {"A", "AND", "B"},
            {"C", "AND", "B"},
            {"D", "NOT", "E"},
            {"B", "OR", "E"},
            {"E", "AND", "END"}
    };

    //This one works!
    char* r1c1 = "A"; char* r1c2 = "AND"; char* r1c3 = "B";
    char* r2c1 = "C"; char* r2c2 = "AND"; char* r2c3 = "B";
    char* r3c1 = "D"; char* r3c2 = "NOT"; char* r3c3 = "E";
    char* r4c1 = "B"; char* r4c2 = "OR";  char* r4c3 = "E";
    char* r5c1 = "E"; char* r5c2 = "END"; char* r5c3 = "END";

    char* row1[] = {r1c1, r1c2, r1c3};
    char* row2[] = {r2c1, r2c2, r2c3};
    char* row3[] = {r3c1, r3c2, r3c3};
    char* row4[] = {r4c1, r4c2, r4c3};
    char* row5[] = {r5c1, r5c2, r5c3};

    char** allRows[] = {row1, row2, row3, row4, row5};


    //This one also works
    char* r1[] = {"A", "AND", "B"};
    char* r2[] = {"C", "AND", "B"};
    char* r3[] = {"D", "NOT", "E"};
    char* r4[] = {"B", "OR", "E"};
    char* r5[] = {"E", "AND", "END"};

    char** combineRows[] = {r1, r2, r3, r4, r5};

    printf("Calling create_gates from simulator\n");
    test_root = create_gates(5, combineRows);         //works
    //*root = create_gates(5, allRows);             //works
    //*root = create_gates(5, test_data_array);     //doesnt work??
    root = &test_root;

    printf("Creating test input data array\n");
    int test_data_inputs[2][5] = {
            {1, 1, 1, 1, 0},
            {0, 0, 0, 1, 1}
    };

    printf("%s\n", (*root).name);
    printf("%s\n", (*root).right->name);

    for(int i = 0; i < 2; i++){
        printf("Start of pass %i\n", i);

        printf("Adding inputs\n");
        add_input_to_tree(test_data_inputs[i], root);

        printf("Simulating\n");
        int result = simulate_tree(root);
        printf("%d\n", result);

        printf("Removing inputs\n");
        remove_inputs_from_tree(root);

        printf("End of pass %i\n", i);
    }

    printf("End of test_data()");
}