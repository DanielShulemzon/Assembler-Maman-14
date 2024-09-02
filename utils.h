#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include "globals.h"

#define SKIP_WHITE_SPACES(string, index) \
    while ((string[(index)]) && ((string[(index)] == '\t') || (string[(index)] == ' '))) { \
        ++(index); \
    }

#define SKIP_WORD(string, index) \
    while ((string[(index)]) && !((string[(index)] == '\t') || (string[(index)] == ' '))) { \
        ++(index); \
    }


char *sum_strings(char *s1, char *s2);
void *malloc_with_check(long size);
void *realloc_with_check(void *ptr1, long size);\
// void remove_new_line(char *str);
int printf_line_error(line_info line, char *message, ...);
instruction get_instruction_by_name(const char *word);
bool is_valid_label_name(const char *name);
bool is_reserved_word(const char *name);
bool is_valid_data_parameter(const char *param);
void free_code_image(machine_word **code_image, long fic);



#endif