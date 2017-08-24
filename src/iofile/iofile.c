//
// Created by ajrgu on 23-Aug-17.
//

#include <stdio.h>
#include <mem.h>
#include <stdlib.h>
#include "iofile.h"

#define BUFFER_SIZE 80
#define CACHE_SIZE 50

// global variables
int currentLine = 0;
unsigned int gatesCacheCount = 0;
MultiGate* gatesCache[CACHE_SIZE];
uint8_t arrayGatesUses[CACHE_SIZE];

// helper functions
static void resetGlobals(void){
    currentLine = 0;
    gatesCacheCount = 0;
    memset(gatesCache, 0, CACHE_SIZE * sizeof(MultiGate*));
    memset(arrayGatesUses, 0, CACHE_SIZE * sizeof(uint8_t));
}

/*
 * Places next line into buffer
 */
static void getNextLine(FILE* fp, char* buffer){
    currentLine++;
    if(fgets(buffer, BUFFER_SIZE, fp) == NULL) {printf("Error reading file at line %d\n", currentLine);}
}

/*
 * Check if line starts with '#'
 */
static int isLineComment(char* line){
    if(line[0] == '#'){
        return 0;
    }
    return 1;
}

/*
 * Splits a string into an array of strings
 */
void strSplit(char* strInput, char** strOutput, int* outputSize, char* splitToken){
    char* p = strtok(strInput, splitToken);
    int i = 0;
    while(p !=NULL)
    {
        strOutput[i++] = p;
        p = strtok(NULL, splitToken);
    }

    *outputSize = i;
}

/*
 * Turns string into gate_type enum
 * OR, AND, XOR, NOT, END
 */
static int stringToGateType(char* gateTypeString){
    int gate_type = -1;

    if(strcmp(gateTypeString, "OR")) gate_type = OR;
    if(strcmp(gateTypeString, "AND")) gate_type = AND;
    if(strcmp(gateTypeString, "XOR")) gate_type = XOR;
    if(strcmp(gateTypeString, "NOT")) gate_type = NOT;
    if(strcmp(gateTypeString, "END")) gate_type = END;

    return gate_type;
}

/*
 * Checks the cache to see if a gate with a given name exists
 * returns pointer to gate, or NULL
 */
static MultiGate* getGate(char* name){
    MultiGate* ptr_gate = NULL;

    for (int i = 0; i < gatesCacheCount; ++i) {
        if(strcmp(gatesCache[i]->name, name)){
            ptr_gate = gatesCache[i];

            // exit loop
            i = gatesCacheCount;
        }
    }

    return ptr_gate;
}

// lines into gates manipulation occurs here
/*
 * Creates a gate from an input line, adds it to gate cache
 */
int lineToGate(char* line, MultiGate* gate){
    unsigned int errors = 0;

    int size = 0;
    char** buffer = malloc(80 * sizeof(char*)) ;
    strSplit(line, buffer, &size, " ");

    gate = (MultiGate*)malloc(sizeof(MultiGate));

    gate->name = buffer[0]; // <- TODO:needs to move from stack to heap
    //strcpy(gate->name, buffer[0]);

    // get gate type and verify
    gate_type temp_gate_type = (gate_type)stringToGateType(buffer[1]);
    if(temp_gate_type == -1) {errors++; printf("Invalid gate type on line %d", currentLine);}

    gate->type = (gate_type)stringToGateType(buffer[1]);
    gate->inputGatesCount = 0;

    // add a pointer on the cache
    gatesCache[gatesCacheCount++] = gate;

    return errors;
}

/*
 * connects gates too each other
 */
int lineToConnection(char* line){
    unsigned int errors = 0;

    int size = 0;
    char** buffer = (char**)malloc(80 * sizeof(char**)) ;
    strSplit(line, buffer, &size, " ");

    // check that the first gate exists
    MultiGate* baseGate = getGate(buffer[0]);
    if(baseGate == NULL){
        printf("Error: cannot create a connection to gate \"%s\" because it doesn't exist. Line: %d",
               buffer[0],
               currentLine);
        errors++;
        // cannot recover from this
        return errors;
    }

    if((size - 1) > MAX_INPUT_GATES){
        printf("Error: too many input gates, MAX_INPUT_GATES = %d. Line: %d", MAX_INPUT_GATES, currentLine);
        errors++;
        return errors;
    }


    if(baseGate->type == NOT && (size - 1) != 1){
        printf("Error: Gate of type NOT must have 1 input. Line: %d", currentLine);
        errors++;
    }

    if(baseGate->type == XOR && (size - 1) != 2){
        printf("Error: Gate of type XOR must have 2 inputs. Line: %d", currentLine);
        errors++;
    }

    // start i at 1 because buffer 0 was already checked for baseGate
    for (int i = 1; i < size; ++i) {
        MultiGate* connectionGate = getGate(buffer[i]);
        // check if gate exists
        if(connectionGate == NULL){
            printf("Error: cannot create a connection to gate \"%s\" because it doesn't exist. Line: %d",
                   buffer[0],
                   currentLine);
            errors++;
            // cannot recover from this
            return errors;
        }

        baseGate->inputGates[baseGate->inputGatesCount++] = connectionGate;
    }

    return errors;
}

int lineToInputs(char* line, Input* input){
    unsigned int errors = 0;

    Input* temp_input = (Input*)malloc(sizeof(Input));

    int i = 0;
    while(line[i] != '\n'){
        if(line[i] == '0'){temp_input->value[temp_input->size++] = 0;}
        if(line[i] == '1'){temp_input->value[temp_input->size++] = 1;}

        ++i;
        if(i > MAX_INPUT_SIZE){
            errors++;
            printf("Error: Input too long. Line: %d", currentLine);
            break;
        }
    }

    return errors;
}

// validation functions
/*
 * Rough validation that checks if GATES, CONNECTIONS and INPUTS sections each occur once
 */
int validateFile(FILE* fp){
    unsigned int errors = 0;

    char buffer[BUFFER_SIZE];

    uint8_t hasGates = 0;
    uint8_t hasConnections = 0;
    uint8_t hasInputs = 0;

    getNextLine(fp, buffer);
    while(!feof(fp)){
        // remove trailing spaces
        sscanf(buffer, "%s", buffer);

        if(strcmp(buffer, "GATES")) hasGates++;
        if(strcmp(buffer, "CONNECTIONS")) hasConnections++;
        if(strcmp(buffer, "INPUTS")) hasInputs++;

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
    fseek(fp, 0, SEEK_SET);

    return errors;
}

/*
 * Checks syntax of gate line
 */
int validateGate(char* line){
    unsigned int errors = 0;

    int size = 0;
    char** buffer = (char**)malloc(80 * sizeof(char**)) ;


    strSplit(line, buffer, &size, " ");

    // check right number of arguments
    if(size != 2){errors++; printf("Gate on line %d does not follow format of <gate_name> <gate_type>\n", currentLine);}

    // check if second argument is a valid gate type
    if(size < 1){
        if(stringToGateType(buffer[1]) == -1){
            errors++;
            printf("Invalid gate_type on line %d, valid types are: OR, AND, XOR, NOT, END\n", currentLine);
        }
    }

    return errors;
}

/*
 * Check syntax of connection line
 */
int validateConnecction(char* line){
    unsigned int errors = 0;

    int size = 0;
    char** buffer = (char**)malloc(80 * sizeof(char**));

    strSplit(line, buffer, &size, " ");

    if(size < 2) {errors++; printf("Invalid connection on line %d: not connected to anything.", currentLine);}

    return errors;
}

/*
 * Checks syntax of input line
 */
int validateInput(char* line){
    unsigned int errors = 0;

    const unsigned int max_input_length = CACHE_SIZE;

    for (int i = 0; i < max_input_length; ++i) {
        if(line[i] == '0') continue;
        if(line[i] == '1') continue;
        if(line[i] == '\n') break;

        printf("Invalid input character at l:%d c:%d. Valid input is '0', '1'.", currentLine, i);
        errors++;
    }

    return errors;
}


// functions that are called by from core
int readFile(char* fileName, MultiGate* endGate, Input** input){
    resetGlobals();

    // open up the file
    FILE *fp;
    fp = fopen(fileName, "r");
    if(fp == NULL){
        printf("Can't open file: %s\n", fileName);
        return -1;
    }

    // rough validation of file
    if(!validateFile(fp)){ return -1; }

    char buffer[BUFFER_SIZE];

    getNextLine(fp, buffer);
    while(!feof(fp)){
        if(isLineComment(buffer)) { continue; }


    }

    return 0;
}

int writeFile(char* fileName, MultiGate* endGate, Output** output){


    return 0;
}
