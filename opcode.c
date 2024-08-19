#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "opcode.h"



typedef struct cmd_element {
    char *cmd;
    opcode op;
}cmd_element;

static cmd_element cmd_lookup_table[] = {
        {"mov", MOV_OP},
        {"cmp",CMP_OP},
        {"add",ADD_OP},
        {"sub",SUB_OP},
        {"lea",LEA_OP},
        {"clr",CLR_OP},
        {"not",NOT_OP},
        {"inc",INC_OP},
        {"dec",DEC_OP},
        {"jmp",JMP_OP},
        {"bne",BNE_OP},
        {"jsr",JSR_OP},
        {"red",RED_OP},
        {"prn",PRN_OP},
        {"rts",RTS_OP},
        {"stop",STOP_OP},
        {NULL, NONE_OP}
};


opcode get_opcode_by_name(const char *name){
    cmd_element *e;

    for(e = cmd_lookup_table; e != NULL; e++){
        if(strcmp(name, e->cmd) == 0) return e->op;
    }
    return NONE_OP;
}



reg get_register_by_name(const char *name){
    int digit;
    if(name[1] && name[0] == 'r' && !name[2]){
        digit = name[1] - '0'; //convert digit to the difference between 0 in ASCII;
        if(digit >= 0 && digit <= 7) return digit;
    }
    return NONE_REG;
}