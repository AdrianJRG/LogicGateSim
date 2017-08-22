//
// Created by Adrian Guillot on 01-Aug-17.
//

/*
 * Unit tests for simulator.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <unity.h>
#include <IOfile.h>
#include <string.h>

void strSplit(char* strInput, char** strOutput, char* splitToken)
{
    char* p = strtok(strInput, splitToken);
    int i = 0;
    while(p !=NULL)
    {
        strOutput[i++] = p;
        p = strtok(NULL, splitToken);
    }
}

void test_lineToContentArray(void){
    printf("Start of test");
    char*** content = (char***)malloc(2 * sizeof(char**));
    int arraySizeContent = 0;

	char* line = (char*)malloc(20*sizeof(char));
    char* lineToBeSplit = "A NOT B";
	stpcpy(line, lineToBeSplit);
	
    lineToContentArray(line, &arraySizeContent, content);

    printf("Begin asserts");
    TEST_ASSERT_EQUAL_STRING("A", content[0][0]);
    TEST_ASSERT_EQUAL_STRING("NOT", content[0][1]);
    TEST_ASSERT_EQUAL_STRING("B", content[0][2]);
    printf("End asserts");

    free(line);

    free(content[0]);

    free(content);

    printf("End of test");
}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_lineToContentArray);

    return UNITY_END();
}
