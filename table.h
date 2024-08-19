#ifndef TABLE_H
#define TABLE_H

#include <stdlib.h>

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

#endif