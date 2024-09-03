/* functions that handle the first pass of the assembler. */
#ifndef _FIRST_PASS_H
#define _FIRST_PASS_H

#include <stdio.h>
#include "globals.h"
#include "table.h"

/*
 *  Processes a line and updates the code and data images.
 *  @param line - information about the line.
 *  @param code_img - the code image.
 *  @param data_img - the data image.
 *  @param ic - the current ic.
 *  @param dc - the current dc.
 *  @param symbol_table - the table which contains the symbols and information about them.
 *  @return - Whether the line is passed the first pass. 
 */
bool fpass_process_line(line_info line, machine_word **code_img, long *data_img, long *ic, long *dc, table *symbol_table);



#endif