#ifndef _SECOND_PASS_
#define _SECOND_PASS_

#include <stdio.h>
#include "globals.h"
#include "table.h"


bool spass_process_line(line_info line, machine_word **code_img, long *ic, table *symbol_table);



#endif