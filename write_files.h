/* function to print the result files of the assembler. */
#ifndef _WRITE_FILES_H
#define _WRITE_FILES_H

#include "globals.h"
#include "table.h"

/* macros to keep only the 15/12 least significant bits in the value. */
#define KEEP_ONLY_15_LSB(value) ((value) & 0x7FFF)

#define KEEP_ONLY_12_LSB(value) ((value) & 0xFFF)


/*
 *  Prints the result files of the assembler.
 *  @param code_img - the code image which contains our code segment.
 *  @param data_img - the data image which contains our data.
 *  @param ic_final - the ic at the end of the process.
 *  @param dc_final - the dc at the end of the process.
 *  @param filename - the name of the original file without it's extention.
 *  @param symbol_table - the table that contains our symbols and their value and types.
 *  @return - Whether succeeded.
 */
bool write_output_files(machine_word **code_img, long *data_img, long ic_final, long dc_final, char *filename,
                       table *symbol_table);

#endif