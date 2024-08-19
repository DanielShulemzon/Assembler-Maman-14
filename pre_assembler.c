#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "pre_assembler.h"
#include "globals.h"
#include "BST.h"
#include "utils.h"

static bool macro_already_exists(char *word, BST *macro_bst);
static void extract_macro_from_node(Node *node, FILE *dest);
void extract_macro_from_node(Node *node, FILE *dest);





static bool pre_assembler_succeeded;



static const char *macro_keyword = "macr", *endmacro_keyword = "endmacr";

bool initiate_pre_assembler(FILE *input_file, FILE *output_file, char *og_filename){
    int i, j = 0;
    BST *macro_bst = create_bst();
    char temp_line[MAX_LINE_LENGTH + 2], tokenized_line[MAX_LINE_LENGTH + 2];
    char *first_word, *macro_name;
    line_info curr_line_info;
    bool inside_macro = false;
    Node *curr_macro;
    Node *temp_node = NULL;

    pre_assembler_succeeded = true;

    curr_line_info.file_name = og_filename;
    // printf("initiating pre_assembler\n");
    for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, input_file) != NULL; curr_line_info.line_number++){
        j = 0;
        curr_line_info.content = temp_line;
        strcpy(tokenized_line, temp_line);

        first_word = strtok(tokenized_line, " \t\n");

        if (strcmp(first_word, macro_keyword) == 0){

            if(!is_macr_legal(curr_line_info, macro_bst)){
                continue;
            }

            strcpy(tokenized_line, temp_line);
            strtok(tokenized_line, " \t\n");
            macro_name = strtok(NULL, " \t\n");

            curr_macro = bst_insert(macro_bst, macro_name);
            // printf("New macro found! macro name: \"%s\" \n", macro_name);
            
            inside_macro = true;
            

            continue;
        }

        if(strcmp(first_word, endmacro_keyword) == 0){
            if(strtok(NULL, " \t\n") != NULL){
                printf_line_error(curr_line_info, "Pre-assembler Error: endmacr does not take operands.");
                pre_assembler_succeeded = false;
            }
            
            inside_macro = false;
            continue;
        }

        if(inside_macro){
            add_line(curr_macro, curr_line_info.content);
            continue;
        }

        if((temp_node = bst_search(macro_bst, first_word)) != NULL){
            // printf("macro replaced. macro name: \"%s\" \n", first_word);
            extract_macro_from_node(temp_node, output_file);
            continue;
        }
        
        fprintf(output_file, "%s", curr_line_info.content);



    }
    free_bst(macro_bst);
    
    if(pre_assembler_succeeded == false) 
        fprintf(stderr, "\npre_assembler failed to ran. see errors up top.\n");
    else
        fprintf(stdout, "\npre_assembler ran successfully.\n");



    return pre_assembler_succeeded;
}

static bool is_macr_legal(line_info macro_info, BST * macro_bst){
    int i = 0;
    char *word, tokenized_line[MAX_LINE_LENGTH + 2];

    strcpy(tokenized_line, macro_info.content);
    strtok(tokenized_line, " \t\n");
    word = strtok(NULL, " \t\n");

    if(word == NULL){
        printf_line_error(macro_info, "Pre-assembler Error: macro name wasn't specified.");
        pre_assembler_succeeded = false;
        return false;
    }

    if(is_reserved_word(word)){
        printf_line_error(macro_info, "Pre-assembler Error: macro name can not be a reserved keyword.");
        pre_assembler_succeeded = false;
        return false;
    }
    
    if(macro_already_exists(word, macro_bst)){
        printf_line_error(macro_info, "Pre-assembler Error: macro name already exists.");
        pre_assembler_succeeded = false;
        return false;
    }

    if(strtok(NULL, " \t\n") != NULL){
        printf_line_error(macro_info, "Pre-assembler Error: macro name can only be one word.");
        pre_assembler_succeeded = false;
        return false;
    }

    return true;
}

static bool macro_already_exists(char *word, BST *macro_bst){
    return bst_search(macro_bst, word) != NULL;
}


static void extract_macro_from_node(Node *node, FILE *dest){
    int i;
    for(i = 0; i < node->line_count; i++){
        if (fprintf(dest, "%s", node->lines[i]) < 0) {
            perror("Error writing to file");
            return;
        }    
    }
}
