/* Impliments a search table for symbols. */
#ifndef _TABLE_H
#define _TABLE_H

#include "globals.h"
#include <stdlib.h>

typedef enum symbol_type {
    CODE_SYMBOL, /* Symbol for code segments. */
    DATA_SYMBOL, /* Symbol for data segments. */
    EXTERNAL_SYMBOL, /* Symbol for external symbols. */
    EXTERNAL_REFERENCE, /* Symbol for external references. */
    ENTRY_SYMBOL /* Symbol for entry points. */
} symbol_type;

/* Represents an entry in the table with a key, value, and symbol type. */
typedef struct {
    char* key; /* Name of the  */
    long value; /* Value associated with the key. */
    symbol_type type; /* Type of the symbol. */
} table_entry;

/*  Represents a dynamic table containing multiple entries */
typedef struct {
    table_entry* entries; /* Array of table entries. */
    size_t size; /* Current number of entries. */
    size_t capacity; /* Maximum number of entries before resizing. */
} table;

/*
 *  Creates a table with an initial capacity.
 *  @param initial_capacity - Initial size for the table.
 *  @return - Pointer to the created table.
 */
table* create_table(size_t initial_capacity);

/*
 *  Adds an entry to the table.
 *  @param tab - Pointer to the table.
 *  @param key - The name of the desired symbol.
 *  @param value - The value associated with the key.
 *  @param type - The type of the symbol.
 */
void add_table_item(table* tab, const char* key, long value, symbol_type type);

/*
 *  Finds an entry by key and filters by symbol types.
 *  @param tab - Pointer to the table.
 *  @param key - The name of the desired symbol.
 *  @param symbol_count - Number of symbol types to filter by.
 *  @return - Pointer to the matching table entry or NULL.
 */
table_entry* find_by_types(table* tab, const char* key, int symbol_count, ...);

/*
 *  Frees memory for the entire table.
 *  @param tab - Pointer to the table to free.
 */
void free_table(table* tab);

/*
 *  Adds a value to all entries of a specific symbol type.
 *  @param tab - Pointer to the table.
 *  @param add_value - Value to add to matching entries.
 *  @param type - Symbol type to filter by.
 */
void add_value_to_symbol_type(table* tab, long add_value, symbol_type type);

/*
 *  Filters the table to include only entries of a specific symbol type.
 *  @param tab - Pointer to the table.
 *  @param type - Symbol type to filter by.
 *  @return - Pointer to the filtered table.
 */
table* filter_table_by_type(table* tab, symbol_type type);

#endif
