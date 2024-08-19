#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "utils.h"
#include "opcode.h"
#include "instructions.h"


static bool is_alphanumberic(const char *name);

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


int printf_line_error(line_info line, char *message, ...) { /* Prints the errors into a file, defined above as macro */
	int result;
	va_list args; /* for formatting */
	/* Print file+line */
	fprintf(stderr,"Error In %s:%ld: ", line.file_name, line.line_number);

	/* use vprintf to call printf from variable argument function (from stdio.h) with message + format */
	va_start(args, message);
	result = vfprintf(stderr, message, args);
	va_end(args);

	fprintf(stderr, "\n");
	return result;
}


instruction get_instruction_by_name(const char *word){
	//ensured word does not point at NULL.
	if(strcmp(word, "data")) return DATA_INST;
	if(strcmp(word, "string")) return STRING_INST;
	if(strcmp(word, "extern")) return EXTERN_INST;
	if(strcmp(word, "entry")) return ENTRY_INST;
	return NONE_INST;
}

bool is_valid_label_name(const char *name, line_info line){ 
	if(!name[0] || strlen(name) > 31){
		printf_line_error(line, "illegal label name: lable name should be between 1-31 letters.");
		return false;
	} 
	if(!(isalpha(name[0]) && is_alphanumeric(name + 1))){
		printf_line_error(line, "illegal label name: first letter should be an alphabetic letter and other letters should be alphanumeric.");
		return false;
	}
	if(is_reserved_word(name))
		printf_line_error(line, "illegal label name: label name could not be a reserved word.");
		return false;
	return true;
}

static bool is_alphanumberic(const char *name){
	int i = 0;
	char ch;
	
	for(i = 0;(ch = name[i]) != '\0' ; i++){
		if(!(isalpha(ch) || isdigit(ch))) return false;
	}

	return true;
}


bool is_reserved_word(const char *name){
	opcode tmp_opcode = get_opcode_by_name(name);
	reg tmp_reg = get_register_by_name(name);

	return tmp_opcode != NONE_OP &&
			tmp_reg != NONE_REG &&
			get_instruction_from_word(name) != NONE_INST &&
			strcmp(name, "macro") != 0 &&
			strcmp(name, "endmacr") != 0;
			
}
