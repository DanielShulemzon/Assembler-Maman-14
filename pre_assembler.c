#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_assembler.h"
#include "globals.h"
#include "MinHeap.h"
#include "utils.h"

static bool pre_assembler_succeeded;

void initate_pre_assembler(FILE *input_file, FILE *output_file){
    int i, j;
    MinHeap *macroHeap = create_heap(HEAP_CAPACITY);
    char temp_line[MAX_LINE_LENGTH + 2], temp_macro_name[MAX_LINE_LENGTH - 2],
        endmacro_keyword[] = "endmacr", macro_keyword[] = "macr";
    line_info curr_line_info;
    char *space;



    for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, input_file) != NULL; curr_line_info.line_number++){
       
        curr_line_info.content = temp_line;
        space = strchr(curr_line_info.content, ' ');
        
        if (strncmp(curr_line_info.content, macro_keyword, sizeof(macro_keyword))){

            if(is_macr_legal(&curr_line_info))

            SKIP_WORD(temp_line, j); SKIP_WHITE_SPACES(temp_line, j);

            strcpy(temp_macro_name, temp_line+j);
            
            printf("New macro found! macro name: %s", temp_macro_name);

        }
    }    
}

bool is_macr_legal(line_info *macro_info){
    int i;
    char *word;
    word = strchr(macro_info->content, ' ');
    word++;

    if(word == NULL){
        fprintf(stderr, "Pre-assembler Error: macro name wasn't specified.");
        pre_assembler_succeeded = false;
        return false;
    }


    


    SKIP_WORD(word, i); SKIP_WHITE_SPACES(word, i);
    if(word[i] != NULL){
        fprintf(stderr, "Pre-assembler Error: macro name can only be one word.");
        pre_assembler_succeeded = false;
        return false;
    }


    return true;
}


bool is_endmacr_legal(char *line){

}
