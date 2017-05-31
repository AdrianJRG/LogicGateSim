/*
 * Things this will do:
 *
 * tell IOfile to save
 * tell IOfile to open
 * tell simulator to simulate
 * tell simulator to add gate_node (build node here or in simulator??)
 * get results from simulator
 * print results
 * save results
 */

/*
 * helper functions for simulate and simulate_save
 */
void setInputFile(char* inputFile){

}

void setOutputFile(char* outputFile){

}

char** getGateArray(){

}

char** getInputArray(){

}
/*
 * End of helper functions
 */

int simulate(char* inputFile){
    /* open file in IOfile */
    setInputFile(inputFile);

    // get_gates() <-- returns pointer to array of char arrays

    // turn those into gates

    // add the gates to tree in simulator
}

int simulateSave(char* inputFile, char* outputFile){
    simulate(inputFile);


}

void showTree(char* inputFile){

}

void saveFile(char* outputFile){

}