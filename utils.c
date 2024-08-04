#include "globals.h"
#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>



char *sum_strings(char *s1, char *s2){
	char *str = (char *)malloc_with_check(strlen(s1) + strlen(s2) + 1);
	strcpy(str, s1);
	strcat(str, s2);
	return str;
}

void *malloc_with_check(long size){
	void *ptr = malloc(size);
	if (ptr == NULL) {
		fprintf(stderr, "Error: Fatal: Memory allocation failed.");
		exit(1);
	}
	return ptr;
}

void *realloc_with_check(void *ptr1, long size){
	void *ptr2 = realloc(ptr1, size);
	if (ptr2 == NULL) {
		fprintf(stderr, "Error: Fatal: Memory allocation failed.");
		exit(1);
	}
	return ptr2;
}


void remove_new_line(char *str) {
    size_t len = strlen(str);
	if(len > 0 && str[len - 1] != '\n')
		return;
	str[len - 1] = '\0';
}