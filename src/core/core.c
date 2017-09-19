//
// Created by ajrgu on 29-Aug-17.
//

#include "core.h"
#include "../iofile/iofile.h"
#include "../simulator/simulator.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

MultiGate endGate;
Input** inputs = NULL; // should be Input* but too much depends on it right now to fix
int inputCount = 0;
Output* outputs = NULL;

// helper functions
static int initializeVariables(void){
    int errors = 0;

    if(inputs == NULL){
        inputs = malloc(MAX_NUMBER_OF_INPUTS * sizeof(Input*));
    } else {
        inputs = realloc(inputs, MAX_NUMBER_OF_INPUTS * sizeof(Input*));
    }

    if(outputs == NULL){
        outputs = malloc(MAX_NUMBER_OF_INPUTS * sizeof(Output));
    } else {
        outputs = realloc(outputs, MAX_NUMBER_OF_INPUTS * sizeof(Output));
    }

    inputCount = 0;

    return errors;
}

/*
 * Used to get an array of external inputs so that their value can more easily adjusted between runs.
 * Tested and working!
 */
static int findInputLocations(MultiGate* gate, MultiGate** inputLocations){
    if(gate->type == INPUT){
        int inputNumber;
        char buffer[80];

        strcpy(buffer, &(gate->name[1])); // skip first character which is '$'
        sscanf(buffer, "%d", &inputNumber);

        inputLocations[inputNumber] = gate;
    } else {
        for (int i = 0; i < gate->inputGatesCount; ++i) {
            findInputLocations(gate->inputGates[i], inputLocations);
        }
    }

    return 0;
}

static int outputToStdOut(Output* output){
    for (int i = 0; i < output->size; ++i) {
        printf("%d", output->value[i]);
    }

    return 0;
}

// actual useful stuff
int loadFromFile(char* fileName, int verbose){
    int errors = 0;

    errors += initializeVariables();
    errors += cleanUpGatesInHeap();
    errors += readFile(fileName, &endGate, inputs, &inputCount, verbose);

    return errors;
}

int saveToFile(char* fileName){
    int errors = 0;

    errors += writeFile(fileName, &endGate, inputs, inputCount, outputs);

    return errors;
}

int cleanHeap(void){
    for (int i = 0; i < inputCount; ++i) {
        free(inputs[inputCount]);
    }
    inputCount = 0;

    return cleanUpGatesInHeap();
}

int simulate(void){
    int errors = 0;

    MultiGate* inputLocations[MAX_NUMBER_OF_INPUTS];
    // first we find where the input locations are and store these in the above array
    findInputLocations(&endGate, inputLocations);

    printf("Starting simulation ...\n");
    printf("Outputs from simulation:\n");
    for (int i = 0; i < inputCount; ++i) {// for every input
        for (int j = 0; j < inputs[i][0].size; ++j) {// we set the external inputs
            inputLocations[j+1]->value = inputs[i][0].value[j]; //TODO: fix "j+1", caused by starting external var at 1.
        }
        simulate_tree(&endGate, &outputs[i]);

        printf("Output[%d]: ", i);
        outputToStdOut(&outputs[i]);
        printf("\n");
    }

    return errors;
}
