#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "utils.h"
#include "globals.h"
#include "pre_assembler.h"
#include "first_pass.h"

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
    bool pre_assembler_succeeded, is_success = true;
    long ic = IC_INIT_VALUE, dc = 0, icf, dcf;
    int i, temp_c;
    char c;
    char *input_file_name, *target_name;
    FILE *input_file, *target;
    line_info curr_line_info;
    char temp_line[MAX_LINE_LENGTH + 2];
    machine_word *code_img[CODE_ARR_IMG_LENGTH];
    long data_img[CODE_ARR_IMG_LENGTH];
    table *symbol_table;

    
    
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
    pre_assembler_succeeded = initiate_pre_assembler(input_file, target, input_file_name);
    if(!pre_assembler_succeeded){
        if (remove(target_name) != 0) {
            perror("Error deleting file");
        return false;
        }
    }
    free(input_file_name);
    fclose(input_file);
    
    // first run.
    //from now on use only target as file.

    symbol_table = create_table(TABLE_INITIAL_CAPACITY); //initialing table capacity.
     
    curr_line_info.file_name = target_name;
    curr_line_info.content = temp_line;

    for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, target) != NULL; curr_line_info.line_number++){
        
        if(strchr(curr_line_info.content, '\n') == NULL && !feof(target)){
            printf_line_error(curr_line_info, "Line too long to process. Maximum line length should be %d.", MAX_LINE_LENGTH);

            is_success == false;
            while ((temp_c = fgetc(target)) != '\n' && temp_c != EOF); // skip leftovers.
        }
        else{
            is_success &= fpass_process_line(curr_line_info, &ic, &dc, code_img, data_img, &symbol_table);
            if(!is_success) icf = -1;
        }
    }




    



    free(target_name);
    fclose(target);

    
    return true;
}