#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "utils.h"
#include "instructions.h"

instruction get_instruction_from_word(const char *word){
    instruction result;

    if(word[0] && word[0] != '.') return NONE_INST;

    if(word + 1 == NULL){
        return ERROR_INST;
    }

    result = get_instruction_by_name(word + 1);

    if(result != NONE_INST) return result;

    return ERROR_INST; /* '.' is mentioned but instruction name is illegal. */
}

bool handle_data_instruction(line_info line, long *dc, long *data_img){
    char *parameters, temp_data[MAX_LINE_LENGTH], *temp_ptr;
    long current_data_value;
    int i, index = 0;

    parameters = strstr(line.content, ".data");
    if(parameters == NULL){
        printf_line_error(line, ".data not found although entered function.");
        exit(1);
    }
    parameters += strlen(".data");

    SKIP_WHITE_SPACES(parameters, index);
    if(parameters[index] == ','){
        printf_line_error(line, "unexpected comma after .data instruction.");
        return false;
    }

    while(parameters[index] != '\n' && parameters[index] != EOF){
        for (i = 0;
		      parameters[index] && !isspace(parameters[index]) &&
		     parameters[index] != ','; index++, i++) {
			temp_data[i] = parameters[index];
		}
        temp_data[i] = '\0'; /* end string */

        if(!is_valid_data_parameter(temp_data)){
            printf_line_error(line, "parameter at .data must be an integer.");
            return false;
        }

        current_data_value = strtol(temp_data, &temp_ptr, 10);

        data_img[*dc] = current_data_value;
        (*dc)++;

        SKIP_WHITE_SPACES(parameters, index);
        if(!parameters[index] || parameters[index] == '\n' || parameters[index] == EOF){
            break; /* end of line. ran successfuly */
        }
        if(parameters[index] == ','){
            index++;
            SKIP_WHITE_SPACES(parameters, index);

            if(parameters[index] == ','){
                printf_line_error(line, "multiple commas in a row.");
                return false;
            }
            if(!parameters[index] || parameters[index] == '\n' || parameters[index] == EOF){
                printf_line_error(line, "line could not end with a comma.");
                return false;
            }
        }
        else{
            printf_line_error(line, "2 arguments without comma between them.");
            return false;
        }
    }
    return true;
}


bool handle_string_instruction(line_info line, long *dc, long *data_img){
    char *arg, temp_c;
    int index = 0;

    arg = strstr(line.content, ".string");
    if(arg == NULL){
        printf_line_error(line, ".string not found.");
        return false;
    }
    arg += strlen(".string");

    SKIP_WHITE_SPACES(arg, index);
    if(arg[index] != '\"'){
        printf_line_error(line, "missing opening quote.");
        return false;
    }
    index++;

    while((temp_c = arg[index]) != '\"'){
        if(!temp_c || temp_c == ' ' || temp_c == '\n'){
            printf_line_error(line, "missing ending quote.");
            return false;
        }

        data_img[*dc] = temp_c;
        (*dc)++;

        index++;
    }
    /* add \0 a the end of the string */
    data_img[*dc] = 0;
    (*dc)++;

    /* check if more than one arguement was given */
    index++;
    SKIP_WHITE_SPACES(arg, index);
    if(arg[index] != ' ' && arg[index] != '\n'){
        printf_line_error(line, "more than one arguement was given to .string instruction.");
        return false;
    }

    return true;
}