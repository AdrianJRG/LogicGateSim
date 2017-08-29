//
// Created by ajrgu on 29-Aug-17.
//

#include "core.h"
#include "../iofile/iofile.h"
#include "../simulator/simulator.h"
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

MultiGate* endGate = NULL;
Input** inputs = NULL;
int inputCount = 0;
Output** outputs = NULL;

// helper functions
static int initializeVariables(void){
    int errors = 0;

    if(inputs == NULL){
        inputs = malloc(MAX_NUMBER_OF_INPUTS * sizeof(Input*));
    } else {
        inputs = realloc(inputs, MAX_NUMBER_OF_INPUTS * sizeof(Input*));
    }

    if(outputs == NULL){
        outputs = malloc(MAX_NUMBER_OF_INPUTS * sizeof(Output*));
    } else {
        outputs = realloc(outputs, MAX_NUMBER_OF_INPUTS * sizeof(Output*));
    }

    return errors;
}


static int findInputLocations(MultiGate* gate, MultiGate** inputLocations){
    if(gate->type == INPUT){
        int inputNumber;
        char buffer[80];

        strcpy(buffer, gate->name + 1); // skip first character which is '$'
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
int loadFromFile(char* fileName){
    int errors = 0;

    errors += initializeVariables();
    errors += readFile(fileName, endGate, inputs, &inputCount);

    return errors;
}

int saveToFile(char* fileName){
    int errors = 0;

    return errors;
}

int simulate(void){
    int errors = 0;

    printf("testfp\n");


    MultiGate* inputLocations[MAX_NUMBER_OF_INPUTS];
    // first we find where the input locations are and store these in the above array
    findInputLocations(endGate, inputLocations);

    for (int i = 0; i < inputCount; ++i) {
        for (int j = 0; j < inputs[i]->size; ++j) {
            inputLocations[j]->value = inputs[i]->value[j];
            // TODO: This might cause issues if inputLocations is not big enough. Make a check for that
        }
        simulate_tree(endGate, outputs[i]);
        printf("Output %d: ", i);
        outputToStdOut(outputs[i]);
        printf("\n");
    }

    return errors;
}
