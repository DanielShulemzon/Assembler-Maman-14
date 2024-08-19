#include <string.h>
#include <stdio.h>
#include <stdlib.h>
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

    return ERROR_INST; //'.' is mentioned but instruction name is illegal.
}