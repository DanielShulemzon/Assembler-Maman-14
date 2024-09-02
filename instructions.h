#ifndef _INSTUCTIONS_H
#define _INSTUCTIONS_H
#include "globals.h"


instruction get_instruction_from_word(const char *word);

bool handle_data_instruction(line_info line, long *dc, long *data_img);

bool handle_string_instruction(line_info line, long *dc, long *data_img);

#endif