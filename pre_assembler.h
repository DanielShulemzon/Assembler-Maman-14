/* Header file for pre_assembler.c */
#ifndef _PRE_ASSEMBLER_H
#define _PRE_ASSEMBLER_H

#include <stdio.h>
#include "globals.h"
#include "BST.h"

/*
 *  Processes a file, looks for macros and extract them.
 *  @param input_file - the file we want to process.
 *  @param output_file - the file post macro extraction.
 *  @param filename - original filename for debugging.
 *  @param macro_bst - binary search tree for macros.
 */
bool initiate_pre_assembler(FILE *input_file, FILE *output_file, char *filename, BST *macro_bst);

#endif