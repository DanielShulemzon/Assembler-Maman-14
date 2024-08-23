#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>
#include "globals.h"

#define MOVE_TO_NOT_WHITE(string, index) \
        for (;string[(index)] && (string[(index)] == '\t' || string[(index)] == ' '); (++(index)))\
        ;

typedef enum symbol_type {
    CODE_SYMBOL,
    DATA_SYMBOL,
    EXTERNAL_SYMBOL,
    EXTERNAL_REFERENCE,
    ENTRY_SYMBOL
} symbol_type;

typedef struct {
    char *key;
    long value;
    symbol_type type;
} table_entry;

typedef struct {
    table_entry *entries;
    size_t size;
    size_t capacity;
} table;

table* create_table(size_t initial_capacity);
void add_table_item(table *tab, const char *key, long value, symbol_type type);
table_entry* find_table_item(table *tab, const char *key);
void free_table(table *tab);
bool key_exists_in_table(table *tab, const char *key);

#endif