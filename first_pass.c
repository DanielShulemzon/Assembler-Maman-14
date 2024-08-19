#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "first_pass.h"
#include "utils.h"
#include "globals.h"


bool fpass_process_line(line_info line, machine_word **code_img, long *data_img, long *ic, long *dc, table *symbol_table){
    bool label_found = false;
    int i,j, length;
    char symbol[MAX_LINE_LENGTH] = {0}, tokenized_line[MAX_LINE_LENGTH + 2], *first_word;
    instruction inst;

    strcpy(tokenized_line, line.content);

    first_word = strtok(tokenized_line, " \t\n");

    if(first_word == NULL || first_word[0] == '\n' || first_word[0] == EOF){
        // the line is empty.
        return true;
    }

    //check if label

    length = strlen(first_word);
    if(length > 0 && first_word[length-1] == ':'){
        // possible label found!

        first_word[length - 1] = '\0'; // remove the ':' from the label


        
        strcpy(symbol, first_word);

        if (!is_valid_label_name(symbol, line)) {
            return false;
        }
        first_word = strtok(NULL, " \t\n");
        
        label_found = true;
    }






}