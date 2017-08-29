//
// Created by Adrian Guillot on 24-Aug-17.
//

#include "iofile-lib-test.h"
//#include <iofile.h>
#include <iofile.c> // need access to global variables in iofile.c for some of the tests
#include <unity.h>
#include <stdlib.h>
#include <string.h>

void test_strSplit(void){
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
    TEST_ASSERT_EQUAL(OR, stringToGateType("OR"));
    TEST_ASSERT_EQUAL(AND, stringToGateType("AND"));
    TEST_ASSERT_EQUAL(XOR, stringToGateType("XOR"));
    TEST_ASSERT_EQUAL(NOT, stringToGateType("NOT"));
    TEST_ASSERT_EQUAL(END, stringToGateType("END"));

    TEST_ASSERT_EQUAL(-1, stringToGateType("SOMETHING"));
}

void test_lineToGate(void){
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
    TEST_IGNORE();
}

void test_lineToInput(void){
    Input* input = malloc(sizeof(Input));

    lineToInputs("100100\n", input);

    TEST_ASSERT_EQUAL_INT(1, input->value[0]);
    TEST_ASSERT_EQUAL_INT(0, input->value[1]);
    TEST_ASSERT_EQUAL_INT(0, input->value[2]);
    TEST_ASSERT_EQUAL_INT(1, input->value[3]);
    TEST_ASSERT_EQUAL_INT(0, input->value[4]);
    TEST_ASSERT_EQUAL_INT(0, input->value[5]);

}

void test_validateFile(void){
    TEST_IGNORE();
}

void test_validateGate(void){
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
    char* line = malloc(80 * sizeof(char));

    char* testString = "01101001\n";
    strcpy(line, testString);
    TEST_ASSERT_EQUAL(0, validateInput(line));
    TEST_ASSERT_EQUAL_STRING("01101001\n", testString);

    char* testString2 = "myGate gate2\n";
    strcpy(line, testString2);
    TEST_ASSERT_NOT_EQUAL(0, validateInput(line));
    TEST_ASSERT_EQUAL_STRING("myGate gate2\n", testString2);

    char* testString3 = "myOtherGate\n";
    strcpy(line, testString3);
    TEST_ASSERT_NOT_EQUAL(0, validateInput(line));
    TEST_ASSERT_EQUAL_STRING("myOtherGate\n", testString3);
}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_strSplit);
    RUN_TEST(test_stringToGateType);
    RUN_TEST(test_lineToGate);
    RUN_TEST(test_lineToConnection);
    RUN_TEST(test_lineToInput);
    RUN_TEST(test_validateFile);
    RUN_TEST(test_validateGate);
    RUN_TEST(test_validateConnection);
    RUN_TEST(test_validateInput);

    return UNITY_END();
}
