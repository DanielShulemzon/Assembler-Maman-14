/* functions to handle instructions. */
#ifndef _INSTUCTIONS_H
#define _INSTUCTIONS_H
#include "globals.h"


/*
 *  Retrieves the instruction corresponding to a given word in a line (with the dot at the start if its an instruction).
 *  @param word - The word representing the instruction.
 *  @return - The corresponding instruction, else: NONE_INST if not starting with a dot, ERROR_INST if starts with a dot but illegal.
 */
instruction get_instruction_from_word(const char *word);

/*
 *  Handles a data instruction, updating the data counter and data image.
 *  @param line - Information about the current line.
 *  @param dc - Pointer to the data counter.
 *  @param data_img - Pointer to the data image array.
 *  @return - True if the instruction was handled successfully, false otherwise.
 */
bool handle_data_instruction(line_info line, long *dc, long *data_img);

/*
 *  Handles a string instruction, updating the data counter and data image.
 *  @param line - Information about the current line.
 *  @param dc - Pointer to the data counter.
 *  @param data_img - Pointer to the data image array.
 *  @return - True if the instruction was handled successfully, false otherwise.
 */
bool handle_string_instruction(line_info line, long *dc, long *data_img);


#endif