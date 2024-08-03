#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_assembler.h"
#include "globals.h"
#include "BST.h"
#include "utils.h"

static bool pre_assembler_succeeded;

static char *illegal_macro_names[22] = {
    "mov", "cmp", "add", "sub", "lea", "clr", "not",
    "inc", "dec","jmp", "bne", "jsr", "red", "prn", 
    "rts", "stop", ".data", ".string", ".entry", ".extern",
    "macro", "endmacr"
};

BST *macro_BST = NULL;

void initiate_pre_assembler(FILE *input_file, FILE *output_file){
    int i, j;
    macro_BST = create_bst();
    char temp_line[MAX_LINE_LENGTH + 2], temp_macro_name[MAX_LINE_LENGTH - 2],
        endmacro_keyword[] = "endmacr", macro_keyword[] = "macr ";
    line_info curr_line_info;
    FILE *curr_location;

    
    copy_file(input_file, output_file);
    curr_location = output_file;

    for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, output_file) != NULL; curr_line_info.line_number++){
        SKIP_WHITE_SPACES(temp_line, j);
       
        curr_line_info.content = temp_line;
        
        if (strncmp(curr_line_info.content + j, macro_keyword, sizeof(macro_keyword))){

            if(!is_macr_legal(&curr_line_info)){
                continue;
            }

            SKIP_WORD(temp_line, j); SKIP_WHITE_SPACES(temp_line, j);

            strcpy(temp_macro_name, temp_line+j);

            go_to_next_line(curr_location);
            point_to_next_word(curr_location);
            insert(macro_BST, temp_macro_name, curr_location);
            printf("New macro found! macro name: %s", temp_macro_name);
            
            continue;
        }

        if(strncmp(curr_line_info.content + j, endmacro_keyword, sizeof(endmacro_keyword))){
            
            continue;
        }

        if()



        go_to_next_line(curr_location);
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

    if(macro_name_in_illeagal_names(word)){
        fprintf(stderr, "Pre-assembler Error: macro name can not be a keyword is assembly.");
        pre_assembler_succeeded = false;
        return false;
    }
    
    if(macro_name_already_exits()){
        fprintf(stderr, "Pre-assembler Error: macro name already exists");
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

bool macro_name_in_illeagal_names(char *word){
    int i;
    for(i = 0; i < sizeof(illegal_macro_names) / sizeof(illegal_macro_names[0]); i++){
        if(strcmp(word, illegal_macro_names[i]) == 0)
        return true;
    }
    return false;
}

bool macro_already_exists(char *word){
    return bst_search(macro_BST, word) != NULL;
}


void copy_file(FILE *source, FILE *dest){
    int c;
    while ((c = fgetc(source)) != EOF) {
        fputc(c, dest);
    }
    if (ferror(source)) {
        perror("Error reading from source file");
    }
}


void go_to_next_line(FILE *file) {
    int ch;
    // Read characters until we reach the end of the line or the end of the file
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            break;
        }
    }
}

void point_to_next_word(FILE *file) {
    int ch;
    // Read characters until we find a non-whitespace character
    while ((ch = fgetc(file)) != EOF) {
        if (!isspace(ch)) {
            // Move the file pointer back by one character to point to the start of the word
            ungetc(ch, file);
            break;
        }
    }
}
