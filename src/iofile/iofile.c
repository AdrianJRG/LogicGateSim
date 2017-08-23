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
MultiGate* gatesCache[CACHE_SIZE];
uint8_t arrayGatesUses[CACHE_SIZE];

// helper functions
static void resetGlobals(void){
    currentLine = 0;
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
void strSplit(char* strInput, char** strOutput, int* outputSize, char* splitToken)
{
    char* p = strtok(strInput, splitToken);
    int i = 0;
    while(p !=NULL)
    {
        strOutput[i++] = p;
        p = strtok(NULL, splitToken);
    }

    *outputSize = i;
}

// validation functions
/*
 * Rough validation that checks if GATES, CONNECTIONS and INPUTS sections each occur once
 */
static int validateFile(FILE* fp){
    char buffer[BUFFER_SIZE];

    uint8_t hasGates = 0;
    uint8_t hasConnections = 0;
    uint8_t hasInputs = 0;

    int errors = 0;

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
    fseek(fp, 0, SEEK_SET);

    return errors;
}

/*
 * Checks syntax of gate line
 */
static int validateGate(char* line){
    int errors = 0;

    int size = 0;
    char** buffer = (char**)malloc(80 * sizeof(char**)) ;


    strSplit(line, buffer, &size, " ");

    // check right number of arguments
    if(size != 2) {errors++; printf("Gate on line %d does not follow format of <gate_name> <gate_type>\n", currentLine);}

    // check if second argument is a valid gate type
    if(size < 1){
        int isValid = 0;
        if(strcmp(buffer[1], "OR")) {isValid++;}
        if(strcmp(buffer[1], "AND")) {isValid++;}
        if(strcmp(buffer[1], "XOR")) {isValid++;}
        if(strcmp(buffer[1], "NOT")) {isValid++;}
        if(strcmp(buffer[1], "END")) {isValid++;}

        if(isValid != 1){errors++; printf("Invalid gate_type on line %d, valid types are: OR, AND, XOR, NOT, END\n", currentLine);}
    }

    return errors;
}

static int validateConnecction(char* line){


    return 0;
}

static int validateInput(char* line){

    return 0;
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
