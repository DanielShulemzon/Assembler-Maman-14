#include "write_files.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 *  Creates a .ob file and prints the binary code into it.
 *  @param code_img - the code image which contains our code segment.
 *  @param data_img - the data image which contains our data.
 *  @param ic_final - the ic at the end of the process.
 *  @param dc_final - the dc at the end of the process.
 *  @param filename - the name of the original file without it's extention.
 *  @return - Whether succeeded
 */
static bool write_ob_file(machine_word** code_img, long* data_img, long ic_final, long dc_final, char* filename);

/*
 *  Creates a .ext/.ent file and prints to it the symbols in a table with their value.
 *  @param tab - the desired table we want to print into the file.
 *  @param filename - the name of the original file without it's extention.
 *  @param file_extention - the file extention of the file we create.
 *  return - Whether succeeded.
 */
static bool write_table_to_file(table* tab, char* filename, char* file_extension);

bool write_output_files(machine_word** code_img, long* data_img, long ic_final, long dc_final, char* filename,
    table* symbol_table)
{
    bool result;

    /* get the filtered tables to create corresponding files. */
    table* externals = filter_table_by_type(symbol_table, EXTERNAL_REFERENCE);
    table* entries = filter_table_by_type(symbol_table, ENTRY_SYMBOL);

    result = write_ob_file(code_img, data_img, ic_final, dc_final, filename) && /* creates the .ob file*/
        write_table_to_file(externals, filename, ".ext") && /* creates the .ext file if needed*/
        write_table_to_file(entries, filename, ".ent"); /* creates the .ent file if needed*/

    /* free the tables. */
    free_table(externals);
    free_table(entries);
    return result;
}

static bool write_ob_file(machine_word** code_img, long* data_img, long ic_final, long dc_final, char* filename)
{
    int i;
    long value = 0;
    FILE* ob_file;
    /* add extension of file to open */
    char* output_filename = sum_strings(filename, ".ob");

    /* open the file. */
    ob_file = fopen(output_filename, "w");
    if (ob_file == NULL) {
        fprintf(stderr, "Error opening file  \"%s\" .\n", output_filename);
        free(output_filename);
        return false;
    }

    fprintf(ob_file, " %ld %ld", ic_final - IC_INIT_VALUE, dc_final);

    /* loop through the code_img array when ic_final - IC_INIT_VALUE is the last index in the image that we filled. */
    for (i = 0; i < ic_final - IC_INIT_VALUE; i++) {
        if (code_img[i]->type == CODE_UNION_TYPE) {
            value = (code_img[i]->word.code->opcode << 11) | (code_img[i]->word.code->src_addressing << 7) | (code_img[i]->word.code->dest_addressing << 3) | (code_img[i]->word.code->ARE);
        } else if (code_img[i]->type == REG_UNION_TYPE) {
            value = (code_img[i]->word.reg->src_register << 6) | (code_img[i]->word.reg->dest_register << 3) | (code_img[i]->word.reg->ARE);
        } else { /* only option left is data. */
            value = (KEEP_ONLY_12_LSB(code_img[i]->word.data->data) << 3) | (code_img[i]->word.data->ARE);
        }

        /* print at least 4 digits of ic, and 5 digits of octal */
        fprintf(ob_file, "\n%.4d %.5lo", i + 100, value);
    }

    /* print the data image. */
    for (i = 0; i < dc_final; i++) {
        /* print only lower 15 bytes */
        value = KEEP_ONLY_15_LSB(data_img[i]);
        /* print at least 4 digits of dc, and 5 digits of octal */
        fprintf(ob_file, "\n%.4ld %.5lo", ic_final + i, value);
    }

    /* Close the file */
    free(output_filename);
    fclose(ob_file);
    return true;
}

static bool write_table_to_file(table* tab, char* filename, char* file_extension)
{
    FILE* dest_file;
    char* output_filename = sum_strings(filename, file_extension);
    int i;

    if (tab->size == 0) {
        return true;
    }
    /* if table contains at least one entry we create the file. */

    dest_file = fopen(output_filename, "w");
    if (dest_file == NULL) {
        fprintf(stderr, "Error opening file  \"%s\" .\n", output_filename);
        free(output_filename);
        return false;
    }

    /* print the first line outside of the loop to avoid unwanted \n. */
    fprintf(dest_file, "%s %.4ld", tab->entries[0].key, tab->entries[0].value);
    for (i = 1; i < tab->size; i++) {
        fprintf(dest_file, "\n%s %.4ld", tab->entries[i].key, tab->entries[i].value);
    }

    /* Close the file */
    free(output_filename);
    fclose(dest_file);
    return true;
}
