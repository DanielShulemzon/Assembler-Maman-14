#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "table.h"
#include "utils.h"
#include "globals.h"
#include "pre_assembler.h"
#include "first_pass.h"
#include "second_pass.h"
#include "write_files.h"

static bool process_file(char *filename);


int main(int argc, char *argv[]) {
    int i;
    bool succeeded = true;

    /*process files by args */
    for(i = 1; i < argc && succeeded; ++i){
        
        succeeded = process_file(argv[i]); /* send the file for full processing. */
    }
    return 0;
}

static bool process_file(char *filename){
    bool pre_assembler_succeeded, is_success = true;
    long ic = IC_INIT_VALUE, dc = 0, ic_final, dc_final;
    char *input_file_name, *target_name;
    FILE *input_file, *target;
    line_info curr_line_info;
    char temp_line[MAX_LINE_LENGTH + 2], temp_c;
    machine_word *code_img[CODE_ARR_IMG_LENGTH];
    long data_img[CODE_ARR_IMG_LENGTH];
    table *symbol_table;

    
    
    input_file_name = sum_strings(filename, ".as");
    input_file = fopen(input_file_name, "r");
    if(input_file == NULL){
        fprintf(stderr, "Error reading file \"%s\". skipping it. \n", input_file_name);
        free(input_file_name); 
        return false;
    }

    
    target_name = sum_strings(filename, ".am");
    target = fopen(target_name, "w+");
    if(target == NULL){
        fprintf(stderr, "Error opening file  \"%s\" .\n", target_name);
        free(target_name); 
        return false;
    }


    /* initiating pre-assembler. */
    pre_assembler_succeeded = initiate_pre_assembler(input_file, target, input_file_name);
    if(!pre_assembler_succeeded){
        if (remove(target_name) != 0) {
            perror("Error deleting file");
        return false;
        }
    }
    free(input_file_name);
    fclose(input_file);

    rewind(target); /* start file from the beginning after pre_assembler. */
    
    /* first_pass. */
    /* from now on we use only target as file. */

    symbol_table = create_table(TABLE_INITIAL_CAPACITY); /* initialing table capacity. */
     
    curr_line_info.file_name = target_name;
    curr_line_info.content = temp_line;

    for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, target) != NULL; curr_line_info.line_number++){
        if(strchr(curr_line_info.content, '\n') == NULL && !feof(target)){
            printf_line_error(curr_line_info, "Line too long to process. Maximum line length is %d.", MAX_LINE_LENGTH);

            is_success = false;
            while ((temp_c = fgetc(target)) != '\n' && temp_c != EOF); /* skip leftovers. */
        }
        else{
            is_success &= fpass_process_line(curr_line_info, code_img, data_img, &ic, &dc, symbol_table);
        }
    }

    /* if first_pass was success, we move on to second_pass. else we go free the memory. */
    if(is_success){
        
        /* save ic and dc. */
        ic_final = ic;
        dc_final = dc;

        ic = IC_INIT_VALUE;

        add_value_to_symbol_type(symbol_table, ic_final, DATA_SYMBOL);

        /* start file from the begining */
        rewind(target);

        for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, target) != NULL; curr_line_info.line_number++){
            is_success &= spass_process_line(curr_line_info, code_img, &ic, symbol_table);
        }

        if(is_success){
            is_success = write_output_files(code_img, data_img, ic_final, dc_final, filename, symbol_table);
        }
    }


    /* free the dynamic variables. */
    free(target_name);
    fclose(target);

    free_table(symbol_table);

    free_code_image(code_img, ic_final);
    
    return is_success;
}