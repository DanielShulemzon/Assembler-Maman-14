#ifndef _WRITE_FILES_H
#define _WRITE_FILES_H

#include "globals.h"
#include "table.h"

#define KEEP_ONLY_15_LSB(value) ((value) & 0x7FFF)

#define KEEP_ONLY_12_LSB(value) ((value) & 0xFFF)

bool write_output_files(machine_word **code_img, long *data_img, long ic_final, long dc_final, char *filename,
                       table *symbol_table);

#endif