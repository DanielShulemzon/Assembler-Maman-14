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




static bool pre_assembler_succeeded;

static char *illegal_macro_names[22] = {
    "mov", "cmp", "add", "sub", "lea", "clr", "not",
    "inc", "dec","jmp", "bne", "jsr", "red", "prn", 
    "rts", "stop", ".data", ".string", ".entry", ".extern",
    "macr", "endmacr"
};

BST *macro_BST = NULL;

static const char *macro_keyword = "macr", *endmacro_keyword = "endmacr";

bool initiate_pre_assembler(FILE *input_file, FILE *output_file){
    int i, j = 0;
    macro_BST = create_bst();
    char temp_line[MAX_LINE_LENGTH + 2], tokenized_line[MAX_LINE_LENGTH + 2];
    char *first_word, *macro_name;
    line_info curr_line_info;
    bool inside_macro = false;
    Node *curr_macro;
    Node *temp_node = NULL;

    pre_assembler_succeeded = true;

    printf("initiating pre_assembler\n");
    for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, input_file) != NULL; curr_line_info.line_number++){
        j = 0;
        curr_line_info.content = temp_line;
        strcpy(tokenized_line, temp_line);

        first_word = strtok(tokenized_line, " \t\n");

        if (strcmp(first_word, macro_keyword) == 0){

            if(!is_macr_legal(&curr_line_info)){
                continue;
            }

            strcpy(tokenized_line, temp_line);
            strtok(tokenized_line, " \t\n");
            macro_name = strtok(NULL, " \t\n");

            curr_macro = bst_insert(macro_BST, macro_name);
            printf("New macro found! macro name: \"%s\" \n", macro_name);
            
            inside_macro = true;
            

            continue;
        }

        if(strcmp(first_word, endmacro_keyword) == 0){
            if(strtok(NULL, " \t\n") != NULL){
                fprintf(stderr, "Pre-assembler Error: endmacr does not take parameters. %d\n", curr_line_info.line_number);
                pre_assembler_succeeded = false;
            }
            
            inside_macro = false;
            continue;
        }

        if(inside_macro){
            add_line(curr_macro, curr_line_info.content);
            continue;
        }

        if((temp_node = bst_search(macro_BST, first_word)) != NULL){
            printf("macro replaced. macro name: \"%s\" \n", first_word);
            extract_macro_from_node(temp_node, output_file);
            continue;
        }
        
        fprintf(output_file, "%s", curr_line_info.content);



    }
    free_bst(macro_BST);
    
    if(pre_assembler_succeeded == false) 
        fprintf(stderr, "\npre_assembler failed to ran. see errors up top.\n");
    else
        fprintf(stdout, "\npre_assembler ran successfully.\n");



    return pre_assembler_succeeded;
}

bool is_macr_legal(line_info *macro_info){
    int i = 0;
    char *word, tokenized_line[MAX_LINE_LENGTH + 2];

    strcpy(tokenized_line, macro_info->content);
    strtok(tokenized_line, " \t\n");
    word = strtok(NULL, " \t\n");

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

    if(strtok(NULL, " \t\n") != NULL){
        fprintf(stderr, "Pre-assembler Error: macro name can only be one word. line %d\n", macro_info->line_number);
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


void extract_macro_from_node(Node *node, FILE *dest){
    int i;
    for(i = 0; i < node->line_count; i++){
        if (fprintf(dest, "%s", node->lines[i]) < 0) {
            perror("Error writing to file");
            return;
        }    
    }
}
