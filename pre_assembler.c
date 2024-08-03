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
    char temp_line[MAX_LINE_LENGTH + 2], temp_macro_name[MAX_LINE_LENGTH - 2], line_without_nl[MAX_LINE_LENGTH + 2];
    line_info curr_line_info;
    bool inside_macro = false;
    Node *curr_macro;
    Node *temp_node = NULL;

    printf("initiating pre_assembler\n");
    for(curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, input_file) != NULL; curr_line_info.line_number++){
        j = 0;
        curr_line_info.content = temp_line;
        strcpy(line_without_nl, curr_line_info.content);
        remove_new_line(line_without_nl);

        SKIP_WHITE_SPACES(curr_line_info.content, j);
        
        if (strncmp(curr_line_info.content + j, macro_keyword, strlen(macro_keyword)) == 0){

            if(!is_macr_legal(&curr_line_info)){
                continue;
            }

            SKIP_WORD(curr_line_info.content, j); SKIP_WHITE_SPACES(curr_line_info.content, j);

            strcpy(temp_macro_name, curr_line_info.content + j);

            bst_insert(macro_BST, temp_macro_name);
            printf("New macro found! macro name: \"%s\" \n", temp_macro_name);
            
            inside_macro = true;
            curr_macro = bst_search(macro_BST, temp_macro_name);
            continue;
        }

        if(strcmp(line_without_nl + j, endmacro_keyword) == 0){
            inside_macro = false;
            continue;
        }

        if(inside_macro){
            add_line(curr_macro, curr_line_info.content);
            continue;
        }

        if((temp_node = bst_search(macro_BST, line_without_nl + j)) != NULL){
            printf("macro replaced. macro name: \"%s\" \n", curr_line_info.content);
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

    strtok(word, " \t\n"); /*ensured token won't be NULL by the first if statement.*/
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
        if(strstr(word, illegal_macro_names[i]) != NULL)
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
