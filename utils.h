#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

#define SKIP_WHITE_SPACES(string, index) \
    do { \
        (++(index)); \
    } while ((string[(index)]) && ((string[(index)] == '\t') || (string[(index)] == ' ')))

#define SKIP_WORD(string, index) \
    do { \
        (++(index)); \
    } while ((string[(index)]) && !((string[(index)] == '\t') || (string[(index)] == ' ')))



char *add_extention_to_string(char *s1, char *s2);
void *malloc_with_check(long size);

#endif