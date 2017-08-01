//
// Created by Adrian Guillot on 01-Aug-17.
//

/*
 * Unit tests for simulator.c
 */

#include "../src/unity.h"
#include "IOfile.h"

void test_lineToContentArray(void){
    char*** content = (char***)malloc(content, 2 * sizeof(char**));
    int arraySizeContent = 0;

    char* lineToBeSplit = "A NOT B";

    lineToContentArray(lineToBeSplit, arraySizeContent, content);

    TEST_ASSERT_EQUAL_STRING("A", content[0][0]);
    TEST_ASSERT_EQUAL_STRING("NOT", content[0][1]);
    TEST_ASSERT_EQUAL_STRING("B", content[0][2]);

    free(content[0]);

    free(content);

}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_lineToContentArray());

    return UNITY_END();
}
