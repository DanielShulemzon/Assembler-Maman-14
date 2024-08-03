#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>

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

#endif