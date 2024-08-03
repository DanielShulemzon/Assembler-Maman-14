#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_assembler.h"
#include "globals.h"
#include "BST.h"
#include "utils.h"

bool is_macr_legal(line_info *macro_info);
bool macro_name_in_illeagal_names(char *word);
bool macro_already_exists(char *word);
void extract_macro_from_node(Node *node, FILE *dest);
void go_to_next_line(FILE *file);
void point_to_next_word(FILE *file);




static bool pre_assembler_succeeded = true;

static char *illegal_macro_names[22] = {
    "mov", "cmp", "add", "sub", "lea", "clr", "not",
    "inc", "dec","jmp", "bne", "jsr", "red", "prn", 
    "rts", "stop", ".data", ".string", ".entry", ".extern",
    "macr", "endmacr"
};

BST *macro_BST = NULL;

static const char *macro_keyword = "macr ", *endmacro_keyword = "endmacr";

bool initiate_pre_assembler(FILE *input_file, FILE *output_file){
    int i, j = 0;
    macro_BST = create_bst();
    char temp_line[MAX_LINE_LENGTH + 2], temp_macro_name[MAX_LINE_LENGTH - 2];
    line_info curr_line_info;
    FILE *curr_location = input_file;
    bool inside_macro = false;
    Node *temp_node = NULL;

    printf("initiating pre_assembler\n");
    for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, input_file) != NULL; curr_line_info.line_number++){
        j = 0;
        curr_line_info.content = temp_line;

        SKIP_WHITE_SPACES(curr_line_info.content, j);
        printf("j after skip: %d\n", j);
        
        if (strncmp(curr_line_info.content + j, macro_keyword, strlen(macro_keyword)) == 0){
            printf("potentially macro: %s", curr_line_info.content);

            if(!is_macr_legal(&curr_line_info)){
                continue;
            }

            SKIP_WORD(curr_line_info.content, j); SKIP_WHITE_SPACES(curr_line_info.content, j);

            strcpy(temp_macro_name, curr_line_info.content + j);

            go_to_next_line(curr_location);
            point_to_next_word(curr_location);
            bst_insert(macro_BST, temp_macro_name, curr_location);
            printf("New macro found! macro name: %s", temp_macro_name);
            
            inside_macro = true;
            continue;
        }

        if(strncmp(curr_line_info.content + j, endmacro_keyword, strlen(endmacro_keyword)) == 0){
            printf("potentially endmacr: %s", curr_line_info.content);
            go_to_next_line(curr_location);
            inside_macro = false;
            continue;
        }

        if(inside_macro){
            printf("potentially inside a macro: %s", curr_line_info.content);
            go_to_next_line(curr_location);
            continue;
        }

        if((temp_node = bst_search(macro_BST, curr_line_info.content + j)) != NULL){
            printf("potentially macro name: %s", curr_line_info.content);
            extract_macro_from_node(temp_node, output_file);
            go_to_next_line(curr_location);
            continue;
        }
        
        printf("potentially ordinary line: %s", curr_line_info.content);
        fprintf(output_file, "%s", curr_line_info.content);



        go_to_next_line(curr_location);
    }
    free_bst(macro_BST);
    
    return pre_assembler_succeeded;
}

bool is_macr_legal(line_info *macro_info){
    int i = 0;
    char *word;
    word = strchr(macro_info->content, ' ');
    word++;

    if(word == NULL){
        fprintf(stderr, "Pre-assembler Error: macro name wasn't specified. line %d\n", macro_info->line_number);
        pre_assembler_succeeded = false;
        return false;
    }

    if(macro_name_in_illeagal_names(word)){
        fprintf(stderr, "Pre-assembler Error: macro name can not be a keyword is assembly. line %d\n", macro_info->line_number);
        pre_assembler_succeeded = false;
        return false;
    }
    
    if(macro_already_exists(word)){
        fprintf(stderr, "Pre-assembler Error: macro name already exists. line %d\n", macro_info->line_number);
        pre_assembler_succeeded = false;
        return false;
    }

    SKIP_WORD(word, i); SKIP_WHITE_SPACES(word, i);
    if(word + i != NULL){
        fprintf(stderr, "Pre-assembler Error: macro name can only be one word. line %d\n", macro_info->line_number);
        pre_assembler_succeeded = false;
        return false;
    }


    return true;
}

bool macro_name_in_illeagal_names(char *word){
    int i; 
    for(i = 0; i < sizeof(illegal_macro_names) / sizeof(illegal_macro_names[0]); i++){
        if(strstr(word, illegal_macro_names[i]) != NULL)
        return true;
    }
    return false;
}

bool macro_already_exists(char *word){
    return bst_search(macro_BST, word) != NULL;
}


void extract_macro_from_node(Node *node, FILE *dest){
    char buf[MAX_LINE_LENGTH + 2] = {0};
    FILE *start_of_macro = node->file;

    while(fgets(buf, sizeof(buf), start_of_macro)){
        if(strstr(buf, endmacro_keyword)){
            return;
        }

        fprintf(dest, "%s", buf);
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