#ifndef _FIRST_PASS_H
#define _FIRST_PASS_H

#include <stdio.h>
#include "globals.h"
#include "table.h"


bool fpass_process_line(line_info line, machine_word **code_img, long *data_img, long *ic, long *dc, table *symbol_table);



#endif