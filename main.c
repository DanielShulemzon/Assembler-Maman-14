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
    char *input_file_name;
    FILE *input_file;
    FILE *post_preprocessor_file;
    
    
    input_file_name = add_extention_to_string(filename, ".as");
    input_file = fopen(input_file_name, "r");
    if(input_file == NULL){
        raiseFileErr(input_file);
        return false;
    }

    
    // initiating first check.
    post_preprocessor_file = initate_pre_assembler(input_file);
    if(input_file == NULL){
        raiseFileErr(input_file);
        return false;
    }
    







    fclose(input_file);
    return true;
}