#include "utils.h"

#define raiseStrErr(string) fprintf(stderr, "Error: something went wrong with \"%s\" .\n", string)
#define raiseFileErr(file_name) fprintf(stderr, "Error: something went wrong with the file \"%s\" .\n", file_name)


char *sum_strings(char *s1, char *s2){
	char *str = (char *)malloc_with_check(strlen(s1) + strlen(s2) + 1);
	strcpy(str, s1);
	strcat(str, s2);
	return str;
}

void *malloc_with_check(long size){
	void *ptr = malloc(size);
	if (ptr == NULL) {
		printf("Error: Fatal: Memory allocation failed.");
		exit(1);
	}
	return ptr;
}