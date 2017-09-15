//
// Created by ajrgu on 23-Aug-17.
//

#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include "iofile.h"

#define BUFFER_SIZE 80
#define CACHE_SIZE 50

// global variables
int currentLine = 0;
INPUT_SECTIONS currentSection = NONE;
unsigned int gatesCacheCount = 0;
MultiGate* gatesCache[CACHE_SIZE];
//uint8_t arrayGatesUses[CACHE_SIZE];

// helper functions

/*
 * Resets global variables. Should be called whenever reading a new file.
 * NOT tested.
 */
static void resetGlobals(void){
    currentLine = 0;
    currentSection = NONE;
    gatesCacheCount = 0;
    memset(gatesCache, 0, CACHE_SIZE * sizeof(MultiGate*));
    //memset(arrayGatesUses, 0, CACHE_SIZE * sizeof(uint8_t));
}

/*
 * Places next line into buffer, strips \n and \r.
 * NOT tested.
 */
static int getNextLine(FILE* fp, char* buffer){
    int errors = 0;

    currentLine++;
    if(fgets(buffer, BUFFER_SIZE, fp) == NULL) {printf("Error reading file at line %d\n", currentLine); errors++;}

    // strip newline
    buffer[strcspn(buffer, "\r\n")] = 0;

    return errors;
}

/*
 * Check if line starts with '#'
 * NOT tested.
 */
static int isLineComment(char* line){
    if(line[0] == '#'){
        printf("Comment detected on line %d, skipping.\n", currentLine);
        return 1;
    }
    return 0;
}

/*
 * Checks if line is empty
 * NOT tested.
 */
int isLineEmpty(char* line){
    if(line[0] == '\0') {
        printf("Empty line detected on line %d, skipping.\n", currentLine);
        return 1;
    }
    return 0;
}

/*
 * Turns string into gate_type enum
 * OR, AND, XOR, NOT, END
 * FULLY tested, working as intended.
 */
int stringToGateType(char* gateTypeString){
    int gate_type = -1;

    if(strcmp(gateTypeString, "OR") == 0) gate_type = OR;
    if(strcmp(gateTypeString, "AND") == 0) gate_type = AND;
    if(strcmp(gateTypeString, "XOR") == 0) gate_type = XOR;
    if(strcmp(gateTypeString, "NOT") == 0) gate_type = NOT;
    if(strcmp(gateTypeString, "END") == 0) gate_type = END;

    return gate_type;
}

/*
 * Checks the cache to see if a gate with a given name exists
 * returns pointer to gate, or NULL
 * Tested, working as intended.
 */
static MultiGate* getGate(char* name){
    MultiGate* ptr_gate = NULL;

    for (int i = 0; i < gatesCacheCount; ++i) {
        if(strcmp(gatesCache[i]->name, name) == 0){
            ptr_gate = gatesCache[i];

            // exit loop
            i = gatesCacheCount;
        }
    }

    return ptr_gate;
}

/*
 * Checks if current line is a header to change to next section
 * Returns 0 if not a section header, else returns 1.
 * NOT tested.
 * TODO: Test this when reading from file (Line endings may be wrong)
 */
int updateSection(char* line){
    if(strcmp(line, "GATES") == 0) {
        currentSection = GATES;
        printf("Start of GATES, line %d.\n", currentLine);
        return 1;
    }
    if(strcmp(line, "CONNECTIONS") == 0) {
        currentSection = CONNECTIONS;
        printf("Start of CONNECTIONS, line %d.\n", currentLine);
        return 1;
    }
    if(strcmp(line, "INPUTS") == 0) {
        currentSection = INPUTS;
        printf("Start of INPUTS, line %d.\n", currentLine);
        return 1;
    }

    return 0;
}

// lines into gates manipulation occurs here
/*
 * Creates a gate from an input line, adds it to gate cache
 * The gate passed to it must be created on heap (malloc)
 * NOT FULLY tested, working as intended.
 * TODO: Finish testing all scenarios for lineToGate.
 */
int lineToGate(char* line, MultiGate* gate){
    unsigned int errors = 0;

    int size = 0;
    char** buffer = malloc(80 * sizeof(char*)) ;
    strSplit(line, buffer, &size, " ");

    // Check if gate already exists
    if(getGate(buffer[0]) != NULL){
        errors++;
        printf("Gates must have unique names. \"%s\" was used more than once. Line: %d\n", buffer[0], currentLine);
        return errors;
    }

    // We create a string in heap, then put the pointer to it in gate.name
    char* gateNameBuffer = (char*)malloc(80 * sizeof(char));
    strcpy(gateNameBuffer, buffer[0]);
    gate->name = gateNameBuffer;

    // get gate type and verify
    gate_type temp_gate_type = (gate_type)stringToGateType(buffer[1]);
    if(temp_gate_type == -1) {
        errors++;
        printf("Invalid gate type. Line: %d\n", currentLine);
        return errors;
    }

    // set to defaults
    gate->type = (gate_type)stringToGateType(buffer[1]);
    gate->inputGatesCount = 0;

    // add a pointer on the cache
    gatesCache[gatesCacheCount++] = gate;

    return errors;
}

/*
 * Connects gates too each other
 * FULLY tested, working as intended.
 */
int lineToConnection(char* line){
    unsigned int errors = 0;
    printf("line: \"%s\" ", line);

    int size = 0;   // number of inputs on the line + name of gate
    char** buffer = (char**)malloc(80 * sizeof(char**)) ;
    strSplit(line, buffer, &size, " ");

    int numberOfInputs = size - 1;
    printf("inputs %d\t\n", numberOfInputs);

    // check that the first gate exists
    MultiGate* baseGate = getGate(buffer[0]);
    if(baseGate == NULL){
        printf("Error: cannot create a connection to gate \"%s\" because it doesn't exist. Line: %d\n",
               buffer[0],
               currentLine);
        errors++;
        // cannot recover from this
        return errors;
    }

    // check if there is space for another input
    if(numberOfInputs > MAX_INPUT_GATES){
        printf("Error: too many input gates, MAX_INPUT_GATES = %d. Line: %d\n", MAX_INPUT_GATES, currentLine);
        errors++;
        return errors;
    }

    // if the gate is of type NOT, there must be one connection
    if(baseGate->type == NOT && numberOfInputs != 1){
        printf("Error: Gate of type NOT must have 1 input. Line: %d\n", currentLine);
        errors++;
        return errors;
    }

    // if the gate is of type XOR, there must be 2 connections
    if(baseGate->type == XOR && numberOfInputs != 2){
        printf("Error: Gate of type XOR must have 2 inputs. Line: %d\n", currentLine);
        errors++;
        return errors;
    }

    // start i at 1 because buffer 0 was already checked for baseGate
    for (int i = 1; i < size; ++i) {
        MultiGate* connectionGate = getGate(buffer[i]);
        // check if connection gate exists and is not an external input
        if(connectionGate == NULL && buffer[i][0] != '$'){
            printf("Error: cannot create a connection to gate \"%s\" because it doesn't exist. Line: %d\n",
                   buffer[0],
                   currentLine);
            errors++;
            // cannot recover from this
            return errors;
        }

        // if its an uncreated input, create input gate
        if(connectionGate == NULL && buffer[i][0] == '$'){
            connectionGate = malloc(sizeof(MultiGate));
            connectionGate->type = INPUT;

            char* gateNameBuffer = (char*)malloc(80 * sizeof(char));
            strcpy(gateNameBuffer, buffer[i]);
            connectionGate->name = gateNameBuffer;
        }

        // line baseGate and connectionGate
        baseGate->inputGates[(baseGate->inputGatesCount)++] = connectionGate;
    }

    return errors;
}

/*
 * Turns input string into input struct
 * NOT FULLY tested, working as intended
 * TODO: Complete test cases
 */
int lineToInputs(char* line, Input* input){
    unsigned int errors = 0;

    int i = 0;
    while(line[i] != '\0'){
        if(line[i] == '0'){input->value[(input->size)++] = 0;}
        else if(line[i] == '1'){input->value[(input->size)++] = 1;}
        else {errors++; printf("Invalid input. Only 0 and 1 are valid inputs. Line %d\n", currentLine);}

        ++i;
        if(i > MAX_INPUT_SIZE){
            errors++;
            printf("Error: Input too long. Line: %d\n", currentLine);
            break;
        }
    }

    //if(i < gatesCacheCount) printf("Input too short, setting remaining inputs to 0.\n");

    return errors;
}

// validation functions
/*
 * Rough validation that checks if GATES, CONNECTIONS and INPUTS sections each occur once
 * NOT tested.
 * TODO: Add tests
 */
int validateFile(FILE* fp){
    int errors = 0;

    char buffer[BUFFER_SIZE];

    uint8_t hasGates = 0;
    uint8_t hasConnections = 0;
    uint8_t hasInputs = 0;

    getNextLine(fp, buffer);
    while(!feof(fp)){
        // remove trailing spaces
        sscanf(buffer, "%s", buffer);

        if(strcmp(buffer, "GATES") == 0) hasGates++;
        if(strcmp(buffer, "CONNECTIONS") == 0) hasConnections++;
        if(strcmp(buffer, "INPUTS") == 0) hasInputs++;

        getNextLine(fp, buffer);
    }


    // check if GATES occurred
    if(hasGates != 1){
        errors++;
        if(hasGates == 0) {printf("A list of gates must be included under a 'GATES' header\n");}
        else { printf("All gates must be under one 'GATES' header\n");}
    }

    // check if CONNECTIONS occurred
    if(hasConnections != 1){
        errors++;
        if(hasConnections == 0) {printf("A list of connections must be included under a 'CONNECTIONS' header\n");}
        else { printf("All connections must be under one 'CONNECTIONS' header\n");}
    }

    // check if INPUTS occurred
    if(hasInputs != 1){
        errors++;
        if(hasInputs == 0) {printf("A list of inputs must be included under a 'INPUTS' header\n");}
        else { printf("All inputs must be under one 'INPUTS' header\n");}
    }

    // go back to start of file
    currentLine = 0;
    errors += fseek(fp, 0, SEEK_SET);

    return errors;
}

/*
 * Checks syntax of gate line
 * FULLY tested, working as intended.
 */
int validateGate(char* line){
    unsigned int errors = 0;

    // split the line
    int size = 0;
    char tempLine[80];
    strcpy(tempLine, line);
    char** buffer = (char**)malloc(80 * sizeof(char**)) ;
    strSplit(tempLine, buffer, &size, " ");

    // check right number of arguments
    if(size != 2){errors++; printf("Gate on line %d does not follow format of <gate_name> <gate_type>\n", currentLine);}

    // check if second argument is a valid gate type
    if(size > 1){
        if(stringToGateType(buffer[1]) == -1){
            errors++;
            printf("Invalid gate_type on line %d, valid types are: OR, AND, XOR, NOT, END\n", currentLine);
        }
    }

    //clean up
    free(buffer);

    return errors;
}

/*
 * Check syntax of connection line
 * FULLY tested, working as intended.
 */
int validateConnection(char* line){
    unsigned int errors = 0;

    char temp_line[80];
    strcpy(temp_line, line);

    int size = 0;
    char** buffer = (char**)malloc(80 * sizeof(char**));

    strSplit(temp_line, buffer, &size, " ");

    if(size < 2) {errors++; printf("Invalid connection on line %d: not connected to anything.\n", currentLine);}
    if(size - 1 > MAX_INPUT_GATES) {
        errors--; printf("Too many inputs. MAX_INPUTS_GATES is %d. Line: %d", MAX_INPUT_GATES, currentLine);
    }

    // clean up
    free(buffer);

    return errors;
}

/*
 * Checks syntax of input line
 * FULLY tested, working as intended.
 */
int validateInput(char* line){
    int errors = 0;

    const unsigned int max_input_length = CACHE_SIZE;

    int i = 0;
    for (i = 0; i < max_input_length; ++i) {
        if(line[i] == '0') continue;
        if(line[i] == '1') continue;
        if(line[i] == '\0') break;

        printf("Invalid input character at l:%d c:%d. Valid input is '0', '1'.\n", currentLine, i);
        errors++;
    }
    if(i > MAX_INPUT_SIZE){
        errors++;
        printf("Input too long at %d. MAX_INPUT_SIZE is %d. Line: %d", i,  MAX_INPUT_SIZE, currentLine);
    }

    return errors;
}


// functions that are called by from core
/*
 * Read from file, creating the needed gates, connections and inputs.
 * PARTIALLY tested.
 */
int readFile(char* fileName, MultiGate* endGate, Input** input, int* inputCount){
    resetGlobals();

    // open up the file
    FILE *fp;
    fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("Can't open file: %s\n", fileName);
        return -1;
    }


    // rough validation of file, file validation itself prints what is wrong.
    if(validateFile(fp) != 0){ return -1; }
    printf("Rough file validation passed...\n");

    // Parse the file and create the gates, connections and inputs
    // create buffer, get first line
    char buffer[BUFFER_SIZE];
    getNextLine(fp, buffer);

    while(!feof(fp)) {
        //printf("l:%3d \t%s\t\n", currentLine, buffer);
        // if line is comment, empty or section header, skip to next line
        if (!isLineComment(buffer) && !isLineEmpty(buffer) && !updateSection(buffer)) {
            switch (currentSection) {
                case NONE:
                    break;
                case GATES:
                    if(validateGate(buffer) != 0){return -1;}
                    MultiGate* gate_ptr = malloc(sizeof(MultiGate));
                    if(lineToGate(buffer, gate_ptr) != 0){ return -1;}
                    break;
                case CONNECTIONS:
                    if(validateConnection(buffer) != 0){ return -1;}
                    if(lineToConnection(buffer) != 0){ return -1;}
                    break;
                case INPUTS:
                    if(validateInput(buffer) != 0){ return -1;}
                    Input* input_ptr = malloc(sizeof(Input));
                    if(lineToInputs(buffer, input_ptr) != 0){ return -1;}
                    input[(*inputCount)++] = input_ptr;
                    break;
                default:
                    printf("\nSomething went horribly wrong in iofile.c\n");
                    return -1;
            }
        }

        // get next line of file
        getNextLine(fp, buffer);
    }

    // link pointer to endGate
    for (int i = 0; i < gatesCacheCount; ++i) {
        if(gatesCache[i]->type == END){
            *endGate = gatesCache[i][0];
            printf("Found end gate at position %d\n", i);
        }
    }

    // to satisfy warning that endGate was not being read.
    if(endGate == NULL) {printf("Houston we have a problem\n"); return -1;}

    // print a summary
    printf("Gates created:\nName\tType\tInputGates\n");
    for (int j = 0; j < gatesCacheCount; ++j) {
        printf("%s\t%d\t%d\n", gatesCache[j]->name, gatesCache[j]->type, gatesCache[j]->inputGatesCount);
    }

    printf("Inputs created:\n");
    for (int k = 0; k < *inputCount; ++k) {
        printf("Input %d: ", k);
        for (int i = 0; i < input[k]->size; ++i) {
            printf("%d", input[k]->value[i]);
        }
        printf("\n");
    }

    return 0;
}


void printGates(FILE *f, MultiGate* gate)
{
    if(gate->type == INPUT) {
        return;
    } else {
        for (int i = 0; i < gate->inputGatesCount; i++) {
            fprintf(f, "%s\t%d\t%d\n", gate->name, gate->type, gate->inputGatesCount);
            printGates(f, gate->inputGates[i]);
        }
    }
}

/*
 * Write output to file.
 * TODO: Test extensively
 * will do
 */
int writeFile(char* fileName, MultiGate* endGate, Output* output){

    FILE *f = fopen(fileName, "w");
    if (f == NULL)
    {
        printf("Error opening file!\n");
    }


    fprintf(f, "Gates :\nName\tType\tInputGates\n");
    for(int i = 0; i < endGate->inputGatesCount; i++) {
        printGates(f, endGate);
    }
    fprintf(f, "OUTPUT:\n");
    for (int i = 0; i < output->size; ++i) { //for each output write
        fprintf(f,"%d ", output->value[i]);
    }

    return 0;
}
