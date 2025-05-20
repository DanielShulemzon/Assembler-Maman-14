/* functions that handle the second pass of the assembler. */
#ifndef _SECOND_PASS_H
#define _SECOND_PASS_H

#include "BST.h"
#include "globals.h"
#include "table.h"
#include <stdio.h>

/*
 *  Processes a line, updates the code image and the data symbol.
 *  @param line - information about the line.
 *  @param code_img - the code image.
 *  @param ic - the current ic.
 *  @param symbol_table - the table which contains the symbols and information about them.
 *  @param macro_bst - binary serach tree for macros.
 *  @return - Whether the line is passed the second pass.
 */
bool spass_process_line(line_info line, machine_word** code_img, long* ic, table* symbol_table, BST* macro_bst);

#endif
