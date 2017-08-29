//
// Created by ajrgu on 29-Aug-17.
//

#include "logicsimlib-test.h"
#include <logicgatelib.h>
#include <unity.h>
#include <stdlib.h>

void test_MultiGate_1(void){
    MultiGate* gate = malloc(sizeof(MultiGate));
    gate->name = "FirstGate";
    gate->type = NOT;

    TEST_ASSERT_EQUAL_STRING("FirstGate", gate->name);
    TEST_ASSERT_EQUAL(NOT, gate->type);
}

int main(void){
    UNITY_BEGIN();

    RUN_TEST(test_MultiGate_1);

    return UNITY_END();
}