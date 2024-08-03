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
        if(succeeded) printf("great success\n");
        else printf("the test is aladeen\n");
    }
    return 0;
}

static bool process_file(char *filename){
    bool is_valid_file = true;
    bool pre_assembler_succeeded;
    int i;
    char c;
    char *input_file_name, *target_name;
    FILE *input_file, *target;
    
    
    input_file_name = sum_strings(filename, ".as");
    input_file = fopen(input_file_name, "r");
    if(input_file == NULL){
        fprintf(stderr, "Error: something went wrong with the file \"%s\" .\n", input_file_name);
        free(input_file_name); 
        return false;
    }

    
    target_name = sum_strings(filename, ".am");
    target = fopen(target_name, "w");
    if(target == NULL){
        fprintf(stderr, "Error: something went wrong with the file \"%s\" .\n", target_name);
        free(target_name); 
        return false;
    }


    // initiating pre-assembler.
    pre_assembler_succeeded = initiate_pre_assembler(input_file, target);
    if(!pre_assembler_succeeded){
        if (remove(target_name) != 0) {
            perror("Error deleting file");
        return false;
        }
    }
    free(input_file_name);
    fclose(input_file);
    





    free(target_name);
    fclose(target);

    
    return true;
}