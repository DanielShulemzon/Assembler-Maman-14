#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h"
#include "globals.h"
#include "pre_assembler.h"

// run command:./main f1.txt f2.txt
static bool process_file(char *filename);


int main(int argc, char *argv[]) {
    int i;
    int c;
    bool succeeded = true;

    for(i = 1; i < argc && succeeded; ++i){

        succeeded = process_file(argv[i]);
    }
    return 0;
}

static bool process_file(char *filename){
    bool is_valid_file = true;
    int i;
    char c;
    char *input_file_name, target_name;
    FILE *input_file, *target;
    
    
    input_file_name = sum_strings(filename, ".as");
    input_file = fopen(input_file_name, "r");
    if(input_file == NULL){
        raiseFileErr(input_file);
        return false;
        free(input_file_name); 
    }

    
    // initiating pre-assembler.
    target_name = sum_strings(filename, ".am");
    target = initiate_pre_assembler(input_file, target);
    if(target == NULL){
        raiseFileErr(input_file);
        return false;
    }
    fclose(input_file);
    







    fclose(input_file);
    return true;
}