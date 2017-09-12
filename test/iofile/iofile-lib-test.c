//
// Created by Adrian Guillot on 24-Aug-17.
//

#include "iofile-lib-test.h"
//#include <iofile.h>
#include <iofile.c> // need access to global variables in iofile.c for some of the tests
#include <unity.h>
#include <stdlib.h>
#include <string.h>
//#include <stdio.h>

/* TEST FILE CONTENTS
GATES
gate1 NOT
gate2 END
gate3 AND

CONNECTIONS
gate1 $1
gate2 gate1 gate3 $3
gate3 $1 $2

#Some comments, these can be placed anywhere

INPUTS
000
001
010
011
100
101
110
111
 */

//Global
char* fileInput = "../../../test-files/test-in.txt";
char* fileOutput = "../../../test-files/test-out.txt";
FILE* fptr;

void test_strSplit(void){
    resetGlobals();

    int size = 0;
    char** buffer = malloc(80 * sizeof(char*));

    char* line = malloc(80 * sizeof(char));
    char* testString = "ABC abc 123";
    strcpy(line, testString);

    strSplit(line, buffer, &size, " ");

    TEST_ASSERT_EQUAL_INT(3, size);
    TEST_ASSERT_EQUAL_STRING("ABC", buffer[0]);
    TEST_ASSERT_EQUAL_STRING("abc", buffer[1]);
    TEST_ASSERT_EQUAL_STRING("123", buffer[2]);

    char* testString2 = "GATE1 NOT";
    strcpy(line, testString2);
    strSplit(line, buffer, &size, " ");

    TEST_ASSERT_EQUAL_INT(2, size);
    TEST_ASSERT_EQUAL_STRING("GATE1", buffer[0]);
    TEST_ASSERT_EQUAL_STRING("NOT", buffer[1]);
}

void test_stringToGateType(void){
    resetGlobals();

    TEST_ASSERT_EQUAL(OR, stringToGateType("OR"));
    TEST_ASSERT_EQUAL(AND, stringToGateType("AND"));
    TEST_ASSERT_EQUAL(XOR, stringToGateType("XOR"));
    TEST_ASSERT_EQUAL(NOT, stringToGateType("NOT"));
    TEST_ASSERT_EQUAL(END, stringToGateType("END"));

    TEST_ASSERT_EQUAL(-1, stringToGateType("SOMETHING"));
}

void test_lineToGate(void){
    resetGlobals();

    MultiGate* gate = malloc(sizeof(MultiGate));
    char* line = malloc(80 * sizeof(char));
    char* testLine = "GATE1 NOT";

    strcpy(line, testLine);

    int errors = lineToGate(line, gate);

    TEST_ASSERT_EQUAL_INT(0, errors);
    TEST_ASSERT_EQUAL_INT(NOT, gate->type);
    TEST_ASSERT_EQUAL_STRING("GATE1", gate->name);

}

void test_lineToConnection(void){
    resetGlobals();

    // First gate
    MultiGate* gate1 = malloc(sizeof(MultiGate));
    char* line1 = malloc(80 * sizeof(char));
    char* testLine1 = "GATE1 AND";
    strcpy(line1, testLine1);
    int errors1 = lineToGate(line1, gate1);
    TEST_ASSERT_EQUAL_INT(0, errors1);

    // Second gate
    MultiGate* gate2 = malloc(sizeof(MultiGate));
    char* line2 = malloc(80 * sizeof(char));
    char* testLine2 = "GATE2 NOT";
    strcpy(line2, testLine2);
    int errors2 = lineToGate(line2, gate2);
    TEST_ASSERT_EQUAL_INT(0, errors2);


    // Create connection
    char* connection1 = malloc(80 * sizeof(char));
    char* testConnection1 = "GATE1 GATE2 $1";
    strcpy(connection1, testConnection1);
    int eCon1 = lineToConnection(connection1);
    TEST_ASSERT_EQUAL_INT(0, eCon1);

    MultiGate* tempGate = getGate("GATE1");
    TEST_ASSERT_NOT_NULL(tempGate);
    TEST_ASSERT_EQUAL_INT(2, tempGate->inputGatesCount);
    TEST_ASSERT_EQUAL_STRING("GATE2", tempGate->inputGates[0]->name);
    TEST_ASSERT_EQUAL_INT(NOT, tempGate->inputGates[0]->type);
    TEST_ASSERT_EQUAL_STRING("$1", tempGate->inputGates[1]->name);
    TEST_ASSERT_EQUAL_INT(INPUT, tempGate->inputGates[1]->type);

    // First gate
    MultiGate* gateA = malloc(sizeof(MultiGate));
    char* lineA = malloc(80 * sizeof(char));
    char* testLineA = "GATE_A AND";
    strcpy(lineA, testLineA);
    int errorsA = lineToGate(lineA, gateA);
    TEST_ASSERT_EQUAL_INT(0, errorsA);

    // Second gate
    MultiGate* gateB = malloc(sizeof(MultiGate));
    char* lineB = malloc(80 * sizeof(char));
    char* testLineB = "GATE_B NOT";
    strcpy(lineB, testLineB);
    int errorsB = lineToGate(lineB, gateB);
    TEST_ASSERT_EQUAL_INT(0, errorsB);

    // Second gate
    MultiGate* gateC = malloc(sizeof(MultiGate));
    char* lineC = malloc(80 * sizeof(char));
    char* testLineC = "GATE_C XOR";
    strcpy(lineC, testLineC);
    int errorsC = lineToGate(lineC, gateC);
    TEST_ASSERT_EQUAL_INT(0, errorsC);

    // Create connection
    char* connectionA = malloc(80 * sizeof(char));
    char* testConnectionA = "GATE_A GATE_B GATE_C $1 $1 $2 GATE1";
    strcpy(connectionA, testConnectionA);
    int eConA = lineToConnection(connectionA);
    TEST_ASSERT_EQUAL_INT(0, eConA);

    tempGate = getGate("GATE_A");
    TEST_ASSERT_NOT_NULL(tempGate);
    TEST_ASSERT_EQUAL_INT(6, tempGate->inputGatesCount);
    TEST_ASSERT_EQUAL_STRING("GATE_B", tempGate->inputGates[0]->name);
    TEST_ASSERT_EQUAL_INT(NOT, tempGate->inputGates[0]->type);
    TEST_ASSERT_EQUAL_STRING("GATE_C", tempGate->inputGates[1]->name);
    TEST_ASSERT_EQUAL_INT(XOR, tempGate->inputGates[1]->type);
    TEST_ASSERT_EQUAL_STRING("$1", tempGate->inputGates[2]->name);
    TEST_ASSERT_EQUAL_INT(INPUT, tempGate->inputGates[2]->type);
    TEST_ASSERT_EQUAL_STRING("$1", tempGate->inputGates[3]->name);
    TEST_ASSERT_EQUAL_INT(INPUT, tempGate->inputGates[3]->type);
    TEST_ASSERT_EQUAL_STRING("$2", tempGate->inputGates[4]->name);
    TEST_ASSERT_EQUAL_INT(INPUT, tempGate->inputGates[4]->type);
    TEST_ASSERT_EQUAL_STRING("GATE1", tempGate->inputGates[5]->name);
    TEST_ASSERT_EQUAL_INT(AND, tempGate->inputGates[5]->type);
    TEST_ASSERT_EQUAL_INT(2, tempGate->inputGates[5]->inputGatesCount);

}

void test_lineToInput(void){
    resetGlobals();

    Input* input = malloc(sizeof(Input));

    lineToInputs("100100", input);

    TEST_ASSERT_EQUAL_INT(1, input->value[0]);
    TEST_ASSERT_EQUAL_INT(0, input->value[1]);
    TEST_ASSERT_EQUAL_INT(0, input->value[2]);
    TEST_ASSERT_EQUAL_INT(1, input->value[3]);
    TEST_ASSERT_EQUAL_INT(0, input->value[4]);
    TEST_ASSERT_EQUAL_INT(0, input->value[5]);
}

void test_validateFile(void){
    resetGlobals();

    fptr = fopen(fileInput, "r");

    TEST_ASSERT_EQUAL_INT(0, validateFile(fptr));

    fclose(fptr);
}

void test_validateGate(void){
    resetGlobals();

    char* line = malloc(80 * sizeof(char));

    char* testString = "myGate NOT";
    strcpy(line, testString);
    TEST_ASSERT_EQUAL(0, validateGate(line));
    TEST_ASSERT_EQUAL_STRING("myGate NOT", testString);

    char* testString2 = "myGate gate2";
    strcpy(line, testString2);
    TEST_ASSERT_NOT_EQUAL(0, validateGate(line));
    TEST_ASSERT_EQUAL_STRING("myGate gate2", testString2);

    char* testString3 = "myOtherGate";
    strcpy(line, testString3);
    TEST_ASSERT_NOT_EQUAL(0, validateGate(line));
    TEST_ASSERT_EQUAL_STRING("myOtherGate", testString3);
}

void test_validateConnection(void){
    resetGlobals();

    char* line = malloc(80 * sizeof(char));

    char* testString = "myGate anotherGate and another";
    strcpy(line, testString);
    TEST_ASSERT_EQUAL(0, validateConnection(line));
    TEST_ASSERT_EQUAL_STRING("myGate anotherGate and another", testString);

    char* testString2 = "myGate gate2";
    strcpy(line, testString2);
    TEST_ASSERT_EQUAL(0, validateConnection(line));
    TEST_ASSERT_EQUAL_STRING("myGate gate2", testString2);

    char* testString3 = "myOtherGate";
    strcpy(line, testString3);
    TEST_ASSERT_NOT_EQUAL(0, validateConnection(line));
    TEST_ASSERT_EQUAL_STRING("myOtherGate", testString3);
}

void test_validateInput(void){
    resetGlobals();

    char* line = malloc(80 * sizeof(char));

    char* testString = "01101001";
    strcpy(line, testString);
    TEST_ASSERT_EQUAL(0, validateInput(line));
    TEST_ASSERT_EQUAL_STRING("01101001", testString);

    char* testString2 = "myGate gate2";
    strcpy(line, testString2);
    TEST_ASSERT_NOT_EQUAL(0, validateInput(line));
    TEST_ASSERT_EQUAL_STRING("myGate gate2", testString2);

    char* testString3 = "myOtherGate";
    strcpy(line, testString3);
    TEST_ASSERT_NOT_EQUAL(0, validateInput(line));
    TEST_ASSERT_EQUAL_STRING("myOtherGate", testString3);
}

void test_getGate(void){
    resetGlobals();
    
    MultiGate* gate1 = malloc(sizeof(MultiGate));
    char* line1 = malloc(80 * sizeof(char));
    char* testLine1 = "GATE1 NOT";
    strcpy(line1, testLine1);
    int errors1 = lineToGate(line1, gate1);

    MultiGate* gate2 = malloc(sizeof(MultiGate));
    char* line2 = malloc(80 * sizeof(char));
    char* testLine2 = "GATE2 OR";
    strcpy(line2, testLine2);
    int errors2 = lineToGate(line2, gate2);
    
    MultiGate* gate3 = malloc(sizeof(MultiGate));
    char* line3 = malloc(80 * sizeof(char));
    char* testLine3 = "GATE3 XOR";
    strcpy(line3, testLine3);
    int errors3 = lineToGate(line3, gate3);

    TEST_ASSERT_EQUAL_INT(0, errors1);
    TEST_ASSERT_EQUAL_INT(0, errors2);
    TEST_ASSERT_EQUAL_INT(0, errors3);

    MultiGate* tempGate = getGate("GATE1");
    if(tempGate == NULL){ TEST_FAIL_MESSAGE("Gate1 was NULL"); return;}
    TEST_ASSERT_EQUAL_STRING("GATE1", tempGate->name);
    TEST_ASSERT_EQUAL_INT(NOT, tempGate->type);
    
    tempGate = getGate("GATE2");
    if(tempGate == NULL){ TEST_FAIL_MESSAGE("Gate2 was NULL"); return;}
    TEST_ASSERT_EQUAL_STRING("GATE2", tempGate->name);
    TEST_ASSERT_EQUAL_INT(OR, tempGate->type);
    
    tempGate = getGate("GATE3");
    if(tempGate == NULL){ TEST_FAIL_MESSAGE("Gate3 was NULL"); return;}
    TEST_ASSERT_EQUAL_STRING("GATE3", tempGate->name);
    TEST_ASSERT_EQUAL_INT(XOR, tempGate->type);

    tempGate = getGate("RandomString");
    TEST_ASSERT_NULL(tempGate);
}

void test_readFile(void){
    resetGlobals();

    MultiGate endGate;
    Input** pInput = malloc(80 * sizeof(Input*));
    int inputCount = 0;

    int errors = readFile(fileInput, &endGate, pInput, &inputCount);

    TEST_ASSERT_EQUAL_INT(0, errors);
    TEST_ASSERT_EQUAL_STRING("gate2", gatesCache[1]->name);
    TEST_ASSERT_NOT_NULL(&endGate);
    TEST_ASSERT_EQUAL_STRING("gate2", endGate.name);
    TEST_ASSERT_EQUAL_INT(3, endGate.inputGatesCount);

    TEST_ASSERT_EQUAL_STRING("gate1", endGate.inputGates[0]->name);
    TEST_ASSERT_EQUAL_STRING("$1", endGate.inputGates[0]->inputGates[0]->name);
}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_strSplit);
    RUN_TEST(test_stringToGateType);
    RUN_TEST(test_getGate);
    RUN_TEST(test_lineToGate);
    RUN_TEST(test_lineToConnection);
    RUN_TEST(test_lineToInput);
    RUN_TEST(test_validateFile);
    RUN_TEST(test_validateGate);
    RUN_TEST(test_validateConnection);
    RUN_TEST(test_validateInput);
    RUN_TEST(test_readFile);

    return UNITY_END();
}
