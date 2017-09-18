//
// Created by ajrgu on 22-Aug-17.
//

#include <stdlib.h>
#include <simulator.h>
#include <unity.h>

void test_simulate_tree(void){
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

    // Output creation
    Output outputs[8];

    // Setup the external inputs
    for (int i = 0; i < 4; ++i) {
        inGate1.value = inputs[i].value[0];
        inGate2.value = inputs[i].value[1];
        inGate3.value = inputs[i].value[2];

        simulate_tree(&endGate, &outputs[i]);
    }

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

    RUN_TEST(test_simulate_tree);

    return UNITY_END();
}
