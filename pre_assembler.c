#include "pre_assembler.h"
#include "globals.h"
#include "utils.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

/*
 *  Checks if the line of initiating a macro is legal.
 *  @param line - current line which contains the definiton of a macro.
 *  @param macro_bst - binary search tree that contains all macros.
 *  @return Whether macro line is legal.
 */
static bool is_macr_legal(line_info line, BST* macro_bst);

/*
 *  Prints the lines contained in a macro to a file.
 *  @param node - a node in a binary search tree which contains the lines inside a macro.
 *  @param dest - the file we want to print to.
 */
static void extract_macro_from_node(Node* node, FILE* dest);

static bool pre_assembler_succeeded;

static const char macro_keyword[] = "macr", endmacro_keyword[] = "endmacr";

bool initiate_pre_assembler(FILE* input_file, FILE* output_file, char* filename, BST* macro_bst)
{
    char temp_line[MAX_LINE_LENGTH + 2], tokenized_line[MAX_LINE_LENGTH + 2];
    char *first_word, *macro_name;
    line_info curr_line_info;
    bool inside_macro = false;
    Node* curr_macro;
    Node* temp_node = NULL;

    pre_assembler_succeeded = true;

    curr_line_info.file_name = filename;

    /* loops through all the source file lines.*/
    for (curr_line_info.line_number = 1; fgets(temp_line, MAX_LINE_LENGTH + 2, input_file) != NULL; curr_line_info.line_number++) {
        curr_line_info.content = temp_line;
        strcpy(tokenized_line, temp_line);

        first_word = strtok(tokenized_line, " \t\n"); /* gets the first word in the line*/

        if (first_word == NULL || first_word[0] == EOF || first_word[0] == ';') { /* empty line - skip it. */
            continue;
        }

        if (strcmp(first_word, macro_keyword) == 0) { /* if first word is the definition of a macro.*/

            if (!is_macr_legal(curr_line_info, macro_bst)) {
                continue;
            }

            strcpy(tokenized_line, temp_line);
            strtok(tokenized_line, " \t\n");
            macro_name = strtok(NULL, " \t\n"); /* get the name of the macro. */

            curr_macro = bst_insert(macro_bst, macro_name); /* insert the macro we found into the binary serach tree and save the current macro.*/

            inside_macro = true;

            continue;
        }

        if (strcmp(first_word, endmacro_keyword) == 0) { /* if first word is endmacr*/
            if (strtok(NULL, " \t\n") != NULL) { /* if 'endmacr' was followed by an operand its an error. */
                printf_line_error(curr_line_info, "Pre-assembler Error: endmacr does not take operands.");
                pre_assembler_succeeded = false;
            }

            inside_macro = false;
            continue;
        }

        if (inside_macro) { /* if found 'macr' and didn't find 'endmacr yet we add the line to the current macro.*/
            add_line(curr_macro, temp_line);
            continue;
        }

        if ((temp_node = bst_search(macro_bst, first_word)) != NULL) {
            /* if we found the name of a macro we print to the dest file the lines inside the macro.*/
            extract_macro_from_node(temp_node, output_file);
            continue;
        }

        fprintf(output_file, "%s", temp_line); /*if it's a regular line we copy it to the dest file. */
    }

    return pre_assembler_succeeded;
}

static bool is_macr_legal(line_info line, BST* macro_bst)
{
    char *word, tokenized_line[MAX_LINE_LENGTH + 2];

    strcpy(tokenized_line, line.content);
    strtok(tokenized_line, " \t\n");
    word = strtok(NULL, " \t\n"); /* get the name of the macro */

    /* return true only if 'macr' was followed by exactly one word and it is legal. */
    if (word == NULL) {
        printf_line_error(line, "Pre-assembler Error: macro name wasn't specified.");
        pre_assembler_succeeded = false;
        return false;
    }

    if (is_reserved_word(word)) {
        printf_line_error(line, "Pre-assembler Error: macro name can not be a reserved keyword.");
        pre_assembler_succeeded = false;
        return false;
    }

    if (bst_search(macro_bst, word) != NULL) { /* if macro name was defined already. */
        printf_line_error(line, "Pre-assembler Error: macro name already exists.");
        pre_assembler_succeeded = false;
        return false;
    }

    if (strtok(NULL, " \t\n") != NULL) { /* if given 2 arguments to 'macr' */
        printf_line_error(line, "Pre-assembler Error: macro name can only be one word.");
        pre_assembler_succeeded = false;
        return false;
    }

    return true;
}

static void extract_macro_from_node(Node* node, FILE* dest)
{
    int i;

    /*loops through all the lines inside the node and prints them to the destination file.*/
    for (i = 0; i < node->line_count; i++) {
        if (fprintf(dest, "%s", node->lines[i]) < 0) {
            perror("Error writing to file");
            return;
        }
    }
}
