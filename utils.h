/* useful functions. */
#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include "globals.h"

/* macro to increment an index until it points to either a character or the end of the line/file. */
#define SKIP_WHITE_SPACES(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;

/*
 *  Concatenates two strings and returns the result.
 *  @param s1 - First string.
 *  @param s2 - Second string.
 *  @return - Pointer to the concatenated string.
 */
char *sum_strings(char *s1, char *s2);

/*
 *  Allocates memory and checks for allocation success.
 *  @param size - Size of the memory to allocate.
 *  @return - Pointer to the allocated memory.
 */
void *malloc_with_check(long size);

/*
 *  Reallocates memory and checks for allocation success.
 *  @param ptr1 - Pointer to the memory block to reallocate.
 *  @param size - New size of the memory block.
 *  @return - Pointer to the reallocated memory.
 */
void *realloc_with_check(void *ptr1, long size);

/*
 *  Prints an error message with line information.
 *  @param line - Structure containing line information.
 *  @param message - Error message to print.
 *  @return - Whether Succeeded.
 */
int printf_line_error(line_info line, char *message, ...);

/*
 *  Retrieves an instruction by its name.
 *  @param word - Name of the instruction.
 *  @return - the instruction if found, otherwide NONE_INST.
 */
instruction get_instruction_by_name(const char *word);

/*
 *  Checks if a string is a valid label name.
 *  @param name - Label name to validate.
 *  @return - True if valid, false otherwise.
 */
bool is_valid_label_name(const char *name);

/*
 *  Checks if a string is a reserved word.
 *  @param name - Word to check.
 *  @return - True if reserved, false otherwise.
 */
bool is_reserved_word(const char *name);

/*
 *  Checks if a string is a valid parameter for .data instruction.
 *  @param param - Data parameter to validate.
 *  @return - True if valid, false otherwise.
 */
bool is_valid_data_parameter(const char *param);

/*
 *  Frees memory allocated for the code image.
 *  @param code_img - Pointer to the code image array.
 *  @param ic_final - Final instruction counter value.
 */
void free_code_image(machine_word **code_img, long ic_final);



#endif