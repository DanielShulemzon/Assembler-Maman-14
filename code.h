#ifndef _CODE_H_
#define _CODE_H_

#include "globals.h"

opcode get_opcode_by_name(const char *name);
reg get_register_by_name(const char *name);
bool extract_operands(line_info line, int index, int *operand_count, char **operands);
code_word *get_code_word(line_info line, opcode curr_opcode, int op_count, char **operands);
data_word *build_data_word(addressing_type addressing, long data, bool is_extern_symbol);
register_word *build_register_word(reg src_register, reg dest_register);


#endif