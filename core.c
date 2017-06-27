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

int sizeContent;
int sizeInput;
char** content[256];
int* input[256];

// root vars
Gate root;
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

// temporary data used for the testing of core
char* r1[] = {"A", "AND", "B"};
char* r2[] = {"C", "AND", "B"};
char* r3[] = {"D", "NOT", "E"};
char* r4[] = {"B", "OR", "E"};
char* r5[] = {"E", "AND", "END"};
char* r6[] = {"A", "AND", "D"};
int i1[] = {0, 1, 1, 1, 0};
int i2[] = {0, 0, 0, 1, 1};

void insertTempData(){
    content[0] = r1;
    content[1] = r2;
    content[2] = r3;
    content[3] = r4;
    content[4] = r5;
    content[5] = r6;

    input[0] = i1;
    input[1] = i2;
}

/*
 * End of helper functions
 */

int simulate(char* inputFile){
    // IOFile.c is currently WIP
    readFile(inputFile, &sizeContent, &sizeInput, *content, input);

    //* Therefore currently data is overwritten
    insertTempData();
    //*/


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
 * temp/test funtions
 */

void test_data(){
    printf("Now in core.c test_data()\n");

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


    //This one also works and is the one that is used
    char* r1[] = {"A", "AND", "B"};
    char* r2[] = {"C", "AND", "B"};
    char* r3[] = {"D", "NOT", "E"};
    char* r4[] = {"B", "OR", "E"};
    char* r5[] = {"E", "AND", "END"};
    char* r6[] = {"A", "AND", "D"};

    char** combineRows[] = {r1, r2, r3, r4, r5};

    char** combineRowsMultipleOutputs[] = {r1, r2, r3, r4, r5, r6};

    printf("Calling create_gates from simulator\n");
    create_gates(5, combineRows, &test_root);         //works when passed a &Gate, not a *Gate

    printf("Calling create_gates with MultiOutputTest!\n");
    create_gates(6, combineRowsMultipleOutputs, &test_root);
    //root = &test_root;
    //create_gates(5, combineRows, root);

    //temporary for debugging
    //return;

    printf("Creating test input data array\n");
    int test_data_inputs[2][5] = {
            {0, 1, 1, 1, 0},
            {0, 0, 0, 1, 1}
    };

//    char* temp_string = test_root.name;
//    if(temp_string == NULL || strcmp(temp_string, "") == 0){
//        printf("Its empty\n");
//    }
//    printf("%s\n", temp_string);
//    printf("After temp\n");

    for(int i = 0; i < 2; i++){
        printf("Start of pass %i\n", i);

        printf("\tAdding inputs\n");
        add_input_to_tree(test_data_inputs[i], 5, &test_root);

        //*
        printf("\tSimulating\n");
        int result = simulate_tree(&test_root);
        printf("\tResult of input %i: %d\n", i, result);

        //*
        printf("\n\tRemoving inputs\n");
        remove_inputs_from_tree(&test_root);

        //*/

        printf("End of pass %i\n", i);
    }

    printf("End of test_data()\n");
}