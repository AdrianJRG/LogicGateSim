//
// Created by Adrian Guillot on 24-Aug-17.
//

#include "iofile-lib-test.h"
#include <iofile.h>
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
}

void test_lineToGate(void){
    MultiGate* gate = malloc(sizeof(MultiGate));
    char* line = malloc(80 * sizeof(char));
    char* testLine = "GATE1 NOT";

    strcpy(line, testLine);

    int value = lineToGate(line, gate);

    TEST_ASSERT_EQUAL_INT(0, value);
    TEST_ASSERT_EQUAL_STRING("GATE1", gate->name);
    TEST_ASSERT_EQUAL_INT(NOT, gate->type);

}

void test_lineToConnection(void){
    TEST_ASSERT_EQUAL_INT(0, 0);

}

void test_lineToInput(void){
    TEST_ASSERT_EQUAL_INT(0, 0);

}

void test_validateFile(void){
    TEST_ASSERT_EQUAL_INT(0, 0);

}

void test_validateGate(void){
    TEST_ASSERT_EQUAL_INT(0, 0);

}

void test_validateConnection(void){
    TEST_ASSERT_EQUAL_INT(0, 0);

}

void test_validateInput(void){
    TEST_ASSERT_EQUAL_INT(0, 0);

}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_strSplit);
    RUN_TEST(test_lineToGate);
    RUN_TEST(test_lineToConnection);
    RUN_TEST(test_lineToInput);
    RUN_TEST(test_validateFile);
    RUN_TEST(test_validateGate);
    RUN_TEST(test_validateConnection);
    RUN_TEST(test_validateInput);

    return UNITY_END();
}
