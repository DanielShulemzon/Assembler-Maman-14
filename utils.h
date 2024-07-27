#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

int copyFiles(char * buf, FILE * f1, FILE * f2);
int countLetters(char *buf, FILE *f1, FILE *f2);
int pointToNextLine(FILE **p, char *buf);

#endif