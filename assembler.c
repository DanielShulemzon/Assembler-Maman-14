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

/*
  Processes a single assembly file, and creates additional result files.
  @param filename - The filename, without it's extension
  @return Whether succeeded
*/
static bool process_file(char *filename);


int main(int argc, char *argv[]) {
    int i;
    bool succeeded = true;

    /*process files by args */
    for(i = 1; i < argc; ++i){
        if(!succeeded) printf("\n"); /* if last test failed, skip a line.*/
        succeeded = process_file(argv[i]); /* send the file for full processing. */
    }
    return 0;
}

static bool process_file(char *filename){
    bool pre_assembler_succeeded, is_success = true;
    /* data and instruction counters. */
    long ic = IC_INIT_VALUE, dc = 0, ic_final, dc_final; 

    /* input file and post pre-assembler file. */
    char *input_file_name, *target_name;
    FILE *input_file, *target;

    line_info curr_line_info;
    char temp_line[MAX_LINE_LENGTH + 2], temp_c;

    /* our  code and data images*/
    machine_word *code_img[CODE_ARR_IMG_LENGTH];
    long data_img[CODE_ARR_IMG_LENGTH];
    table *symbol_table; /* table that contains all labels and their types*/
    BST *macro_bst; /* Binary seach tree that contains all macros. */


    
    /* get the source file name and open the file.*/
    input_file_name = sum_strings(filename, ".as");
    input_file = fopen(input_file_name, "r");
    if(input_file == NULL){ /* if file not found or an error occoured.*/
        fprintf(stderr, "Error reading file \"%s\". skipping it. \n", input_file_name);
        free(input_file_name); 
        return false;
    }

    /* open a new file for post pre-assembler */
    target_name = sum_strings(filename, ".am");
    target = fopen(target_name, "w+");
    if(target == NULL){
        fprintf(stderr, "Error opening file  \"%s\" .\n", target_name);
        free(target_name); 
        return false;
    }

    macro_bst = create_bst(); /* initiate the binary search tree.*/
    /* initiating pre-assembler. */
    pre_assembler_succeeded = initiate_pre_assembler(input_file, target, input_file_name, macro_bst);
    if(!pre_assembler_succeeded){
        if (remove(target_name) != 0) { /* if pre-assembler failed we delete the .am file.*/
            perror("Error deleting file");
        }
        return false;
    }
    free(input_file_name);
    fclose(input_file); /* we close the input file because from now on we will use the target file */

    rewind(target); /* start file from the beginning after pre_assembler. */
    
    /* first_pass. */

    symbol_table = create_table(TABLE_INITIAL_CAPACITY); /* initialing table capacity. */
     
    curr_line_info.file_name = target_name;
    curr_line_info.content = temp_line;

    /* loops through all the lines of the file and processes it.*/
    for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, target) != NULL; curr_line_info.line_number++){
        if(strchr(curr_line_info.content, '\n') == NULL && !feof(target)){
            printf_line_error(curr_line_info, "Line too long to process. Maximum line length is %d.", MAX_LINE_LENGTH);

            is_success = false;
            while ((temp_c = fgetc(target)) != '\n' && temp_c != EOF); /* skip leftovers. */
        }
        else{
            /* will be true only if each line was processed successfuly. */
            is_success &= fpass_process_line(curr_line_info, code_img, data_img, &ic, &dc, symbol_table, macro_bst); 
        }
    }

    /* if first_pass was success, we move on to second_pass. else we go free the memory. */
    if(is_success){
        
        /* save ic and dc. */
        ic_final = ic;
        dc_final = dc;

        ic = IC_INIT_VALUE;

        /* update all the values of labels that contain data. */
        add_value_to_symbol_type(symbol_table, ic_final, DATA_SYMBOL);

        /* start file from the begining */
        rewind(target);

        /* loops through all the lines again and updates the code image.*/
        for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, target) != NULL; curr_line_info.line_number++){
            is_success &= spass_process_line(curr_line_info, code_img, &ic, symbol_table, macro_bst);
        }

        /* if second pass ran successfuly we write the .ob, .ent, .ext files. */
        if(is_success){
            is_success = write_output_files(code_img, data_img, ic_final, dc_final, filename, symbol_table);
        }
    }


    /* free the dynamic variables. */
    free(target_name);
    fclose(target);

    free_table(symbol_table);

    free_bst(macro_bst);

    free_code_image(code_img, ic_final);
    
    return is_success;
}