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

int sizeContent = 0;
int sizeInput = 0;
char*** content = NULL;
int** input = NULL;

// root vars
Gate root;
Gate test_root;

/*
 * helper functions
 */

void strSplit(char* strInput, char** strOutput, char* splitToken)
{
	char* p = strtok(strInput, splitToken);
	int i = 0;
	while(p !=NULL)
	{	
		strOutput[i++] = p;
		p = strtok(NULL, splitToken);
	}
}

void initializeArrays(){
    content = (char***)realloc(content, 256*sizeof(char**));
    input = (int**)realloc(input, 256*sizeof(int*));
}

int gateToString(char* buffer, char** gate){
    strcpy(buffer, "");
    strcat(buffer, gate[0]);
    strcat(buffer, " ");
    strcat(buffer, gate[1]);
    strcat(buffer, " ");
    strcat(buffer, gate[2]);

    return 0;
}

int inputToString(char* buffer, int* inputS){
    strcpy(buffer, "");
    int i = 0;
    char temp[8];
    while(inputS[i] != -1){
        sprintf(temp, "%i", inputS[i]);
        strcat(buffer, temp);
        i++;
    }

    return 0;
}

int getSizeOfInput(int* inputS){
    int i = 0;
    while (inputS[i] != -1){
        i++;
    }
    return i;
}

/*
 * End of helper functions
 */

int simulate(char* inputFile){

    // initialize content and input arrays
    initializeArrays();

    // read from file
    readFromFile(inputFile, &sizeContent, &sizeInput, content, input);

    // Debugging info
    /*
    for (int b = 0; b < 2; ++b) {
        printf("content print from core: %s", content[b][0]);
        printf(" %s", content[b][1]);
        printf(" %s\n", content[b][2]);
    }
    for (int i = 0; i < 3; ++i) {
        printf("input[%i] print from core: %i", i, input[i][0]);
        int j = 1;
        while(input[i][j] != -1) {
            printf(" %i", input[i][j]);
            j++;
        }
        printf("\n");
    }
    //*/

    // turn the strings into gates
    int gatesGood = create_gates(sizeContent, content, &root);

    if(gatesGood == -1){
        printf("Aborting simulation\n");
    } else {
        // in inputs through gates
        for (int k = 0; k < sizeInput; ++k) {
            char inputString[128];
            int result;

            inputToString(inputString, input[k]);
            printf("input %i (%s), ", k, inputString);

            add_input_to_tree(input[k], getSizeOfInput(input[k]), &root);
            result = simulate_tree(&root);
            remove_inputs_from_tree(&root);

            printf("simulation result: %i\n", result);
        }
    }

    freeGates(&root);

    return 0;
}

int simulateSave(char* inputFile, char* outputFile){
    // initialize content and input arrays
    initializeArrays();

    // read from file
    readFromFile(inputFile, &sizeContent, &sizeInput, content, input);

    // Debugging info
    /*
    for (int b = 0; b < 2; ++b) {
        printf("content print from core: %s", content[b][0]);
        printf(" %s", content[b][1]);
        printf(" %s\n", content[b][2]);
    }
    for (int i = 0; i < 3; ++i) {
        printf("input[%i] print from core: %i", i, input[i][0]);
        int j = 1;
        while(input[i][j] != -1) {
            printf(" %i", input[i][j]);
            j++;
        }
        printf("\n");
    }
    //*/

    int linesForFormatting = 10;
    int linesNeeded = sizeContent + sizeInput + linesForFormatting;
    int bufferArrayCounter = 0;
    char** bufferArray = (char**)malloc(sizeof(char*)*linesNeeded);
    char* lineBuffer;

    lineBuffer = (char*)malloc(sizeof(char)*80);
    sprintf(lineBuffer, "Simulation output of %i gates and %i inputs", sizeContent, sizeInput);
    bufferArray[bufferArrayCounter++] = lineBuffer;

    lineBuffer = (char*)malloc(sizeof(char)*80);
    sprintf(lineBuffer, "Gates used:");
    bufferArray[bufferArrayCounter++] = lineBuffer;

    for (int i = 0; i < sizeContent; ++i) {
        char temp[30];
        gateToString(temp, content[i]);

        lineBuffer = (char*)malloc(sizeof(char)*80);
        sprintf(lineBuffer, "\t%s", temp);
        bufferArray[bufferArrayCounter++] = lineBuffer;
    }

    // turn the strings into gates
    int gatesGood = create_gates(sizeContent, content, &root);

    if(gatesGood == -1){
        lineBuffer = (char*)malloc(sizeof(char)*80);
        sprintf(lineBuffer, "\nError:\tDid not find END gate. Aborting simulation");
        bufferArray[bufferArrayCounter++] = lineBuffer;
    } else {
        lineBuffer = (char*)malloc(sizeof(char)*80);
        sprintf(lineBuffer, "Simulation results:");
        bufferArray[bufferArrayCounter++] = lineBuffer;
        // in inputs through gates
        for (int k = 0; k < sizeInput; ++k) {
            char inputString[128];
            int result;

            inputToString(inputString, input[k]);
            printf("input %i (%s), ", k, inputString);

            add_input_to_tree(input[k], getSizeOfInput(input[k]), &root);
            result = simulate_tree(&root);
            remove_inputs_from_tree(&root);

            printf("simulation result: %i\n", result);

            lineBuffer = (char*)malloc(sizeof(char)*80);
            sprintf(lineBuffer, "\tinput %i (%s), simulation result: %i", k, inputString, result);
            bufferArray[bufferArrayCounter++] = lineBuffer;
        }
    }
    // write everything to file
    writeToFile(outputFile, bufferArrayCounter, bufferArray);

    // clean up
    for (int j = 0; j < bufferArrayCounter; ++j) {
        free(bufferArray[bufferArrayCounter]);
    }
    free(bufferArray);

    freeGates(&root);

    return 0;
}

void showTree(char* inputFile){
    printf("showTree not implemented");
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
