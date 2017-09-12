//
// Created by ajrgu on 22-Aug-17.
//

#include <stdlib.h>
#include <core.c> // need to probe global vars
#include <iofile.c>
#include <logicgatelib.h>
#include <unity.h>

//Globals
char* fileInput = "../../../test-files/test-in.txt";
char* fileOutput = "../../../test-files/test-out.txt";

void test_showTree(void){
    TEST_IGNORE();
}

void test_loadFromFile(void){
    int errors = loadFromFile(fileInput);

    TEST_ASSERT_EQUAL_INT(0, errors);
    TEST_ASSERT_EQUAL_STRING("gate2", endGate.name);
    TEST_ASSERT_EQUAL_STRING("gate1", endGate.inputGates[0]->name);

    TEST_ASSERT_EQUAL_STRING("$3", endGate.inputGates[2]->name);
    TEST_ASSERT_EQUAL_INT(INPUT, endGate.inputGates[2]->type);

    TEST_ASSERT_EQUAL_INT(0, inputs[2]->value[0]);
    TEST_ASSERT_EQUAL_INT(1, inputs[2]->value[1]);
    TEST_ASSERT_EQUAL_INT(0, inputs[2]->value[2]);

    TEST_ASSERT_EQUAL_INT(1, inputs[6]->value[0]);
    TEST_ASSERT_EQUAL_INT(1, inputs[6]->value[1]);
    TEST_ASSERT_EQUAL_INT(0, inputs[6]->value[2]);

    TEST_ASSERT_EQUAL_INT(8, inputCount);
}

void test_findInputLocations(void){
    // gate creation
    MultiGate endGate;
    endGate.name = "gate2";
    endGate.type = END;
    endGate.inputGatesCount = 0;

    MultiGate notGate;
    notGate.name = "gate1";
    notGate.type = NOT;
    notGate.inputGatesCount = 0;

    MultiGate andGate;
    andGate.name = "gate3";
    andGate.type = AND;
    andGate.inputGatesCount = 0;

    MultiGate inGate1;
    inGate1.name = "$1";
    inGate1.type = INPUT;
    inGate1.inputGatesCount = 0;

    MultiGate inGate2;
    inGate2.name = "$2";
    inGate2.type = INPUT;
    inGate2.inputGatesCount = 0;

    MultiGate inGate3;
    inGate3.name = "$3";
    inGate3.type = INPUT;
    inGate3.inputGatesCount = 0;

    // connection creation
    notGate.inputGates[notGate.inputGatesCount++] = &inGate1;

    endGate.inputGates[endGate.inputGatesCount++] = &notGate;
    endGate.inputGates[endGate.inputGatesCount++] = &andGate;
    endGate.inputGates[endGate.inputGatesCount++] = &inGate3;

    andGate.inputGates[andGate.inputGatesCount++] = &inGate1;
    andGate.inputGates[andGate.inputGatesCount++] = &inGate2;

    // input creation
    Input inputs[8];
    inputs[0].size = 3;
    inputs[0].value[0] = 0;
    inputs[0].value[1] = 0;
    inputs[0].value[2] = 0;

    inputs[1].size = 3;
    inputs[1].value[0] = 0;
    inputs[1].value[1] = 0;
    inputs[1].value[2] = 1;

    inputs[2].size = 3;
    inputs[2].value[0] = 0;
    inputs[2].value[1] = 1;
    inputs[2].value[2] = 0;

    inputs[3].size = 3;
    inputs[3].value[0] = 0;
    inputs[3].value[1] = 1;
    inputs[3].value[2] = 1;

    //setup for findInputLocations
    MultiGate* inputLocations[80];

    findInputLocations(&endGate, inputLocations);

    TEST_ASSERT_EQUAL_STRING("$1", inputLocations[1]->name);
    TEST_ASSERT_EQUAL_STRING("$2", inputLocations[2]->name);
    TEST_ASSERT_EQUAL_STRING("$3", inputLocations[3]->name);

    inGate1.value = 0;
    TEST_ASSERT_EQUAL_INT(0, inputLocations[1]->value);
    inputLocations[1]->value = 1;
    TEST_ASSERT_EQUAL_INT(1, inputLocations[1]->value);
}

void test_simulate(void){
    int errors = loadFromFile(fileInput);
    errors += simulate();
    TEST_ASSERT_EQUAL_INT(0, errors);

    TEST_ASSERT_EQUAL_INT(3, outputs[0].size);
    TEST_ASSERT_EQUAL_INT(1, outputs[0].value[0]);
    TEST_ASSERT_EQUAL_INT(0, outputs[0].value[1]);
    TEST_ASSERT_EQUAL_INT(0, outputs[0].value[2]);

    TEST_ASSERT_EQUAL_INT(3, outputs[1].size);
    TEST_ASSERT_EQUAL_INT(1, outputs[1].value[0]);
    TEST_ASSERT_EQUAL_INT(0, outputs[1].value[1]);
    TEST_ASSERT_EQUAL_INT(1, outputs[1].value[2]);

    TEST_ASSERT_EQUAL_INT(3, outputs[2].size);
    TEST_ASSERT_EQUAL_INT(1, outputs[2].value[0]);
    TEST_ASSERT_EQUAL_INT(0, outputs[2].value[1]);
    TEST_ASSERT_EQUAL_INT(0, outputs[2].value[2]);

    TEST_ASSERT_EQUAL_INT(3, outputs[3].size);
    TEST_ASSERT_EQUAL_INT(1, outputs[3].value[0]);
    TEST_ASSERT_EQUAL_INT(0, outputs[3].value[1]);
    TEST_ASSERT_EQUAL_INT(1, outputs[3].value[2]);
}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_showTree);
    RUN_TEST(test_loadFromFile);
    RUN_TEST(test_findInputLocations);
    RUN_TEST(test_simulate); // TODO: fix segfault.

    return UNITY_END();
}
