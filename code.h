/* functions to handle code lines. */
#ifndef _CODE_H
#define _CODE_H

#include "globals.h"

/*
 *  Retrieves the opcode corresponding to a given name.
 *  @param name - The name of the opcode.
 *  @return - The opcode corresponding to the name, else NONE_OP.
 */
opcode get_opcode_by_name(const char *name);

/*
 *  Retrieves the register corresponding to a given name.
 *  @param name - The name of the register.
 *  @return - The register corresponding to the name, else NONE_REG.
 */
reg get_register_by_name(const char *name);

/*
 *  Extracts operands from a line of code and stores them in the provided array.
 *  @param line - Information about the current line.
 *  @param index - Index in the line where the operands start.
 *  @param operand_count - Pointer to store the number of operands found.
 *  @param operands - Array to store the extracted operands.
 *  @return - True if operands were successfully extracted, false otherwise.
 */
bool extract_operands(line_info line, int index, int *operand_count, char operands[2][MAX_LINE_LENGTH]);

/*
 *  Determines the addressing type of an operand.
 *  @param operand - The operand string.
 *  @return - The addressing type of the operand.
 */
addressing_type get_addressing_type(char *operand);

/*
 *  Creates a code word based on the opcode, operands, and their count.
 *  @param line - Information about the current line.
 *  @param curr_opcode - The opcode of the instruction.
 *  @param op_count - Number of operands.
 *  @param operands - Array of operand strings.
 *  @return - Pointer to the created code word.
 */
code_word *get_code_word(line_info line, opcode curr_opcode, int op_count, char operands[2][MAX_LINE_LENGTH]);

/*
 *  Builds a data word based on the addressing type, data, and external symbol flag.
 *  @param addressing - The addressing type.
 *  @param data - The data value to store.
 *  @param is_extern_symbol - Flag indicating if the symbol is external.
 *  @return - Pointer to the created data word.
 */
data_word *build_data_word(addressing_type addressing, long data, bool is_extern_symbol);

/*
 *  Builds a register word based on the source and destination registers.
 *  @param src_register - The source register.
 *  @param dest_register - The destination register.
 *  @return - Pointer to the created register word.
 */
register_word *build_register_word(reg src_register, reg dest_register);

#endif