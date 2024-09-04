#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include "utils.h"
#include "code.h"
#include "instructions.h"

/*
 *  Checks if name is alphanumeric.
 *  @param word - the word we want to check.
 *  @return - whether it is alphanumeric.
 */
static bool is_alphanumeric(const char *word);

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
	fprintf(stderr,"Error In %s:%u: ", line.file_name, line.line_number);

	/* use vprintf to call printf from variable argument function (from stdio.h) with message + format */
	va_start(args, message);
	result = vfprintf(stderr, message, args);
	va_end(args);

	fprintf(stderr, "\n");
	return result;
}


instruction get_instruction_by_name(const char *word){
	if(word == NULL) return NONE_INST;

	if(strcmp(word, "data") == 0) return DATA_INST;

	if(strcmp(word, "string") == 0) return STRING_INST;

	if(strcmp(word, "extern") == 0) return EXTERN_INST;

	if(strcmp(word, "entry") == 0) return ENTRY_INST;

	return NONE_INST;
}


bool is_valid_label_name(const char *name){ 

	if(!name[0] || strlen(name) > 31){ /* label name must be between 1 and 31 characters. */
		return false;
	} 
	if(!(isalpha(name[0]) && is_alphanumeric(name + 1))){ /* label name must start in a character and the rest must be either a character or a nubmer.*/
		return false;
	}
	if(is_reserved_word(name)) /* label name could not be a reserved word.*/
		return false;
	return true;
}

static bool is_alphanumeric(const char *word){
	int i = 0;
	char ch;
	
	for(i = 0;(ch = word[i]) != '\0' ; i++){
		if(!(isalpha(ch) || isdigit(ch))) return false;
	}

	return true;
}


bool is_reserved_word(const char *word){
	instruction inst;

	return get_opcode_by_name(word) != NONE_OP || /* if it's an opcode */
			get_register_by_name(word) != NONE_REG || /* if it's a register */
			(((inst = get_instruction_from_word(word)) != NONE_INST) && (inst != ERROR_INST)) || /* if it's a legal instruction*/
			strcmp(word, "macro") == 0 ||
			strcmp(word, "endmacr") == 0;
			
}

bool is_valid_data_parameter(const char *param){
	int i;

	if(param[0] == '+' || param[0] == '-') param++;

	for(i = 0; param[i]; i++){
		if(!isdigit(param[i])) return false;
	}
	return i > 0; /* returns false if param is either empty or only a +/- sign*/
}

void free_code_image(machine_word **code_img, long ic_final) {
	long i;
	/* for each not-null cell (we might have some "holes", so we won't stop on first null) */
	for (i = 0; i <= ic_final; i++) {
		machine_word *curr_word = code_img[i];
		if (curr_word != NULL) {
			/* free code/data/reg word */
			if (curr_word->type == CODE_UNION_TYPE) {
				free(curr_word->word.code);
			}
			else if(curr_word->type == REG_UNION_TYPE) {
				free(curr_word->word.reg);
			}
			else{ /* only option left is data */
				free(curr_word->word.data);
			}
			free(curr_word);
			code_img[i] = NULL;
		}
	}
}