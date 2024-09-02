#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "table.h"
#include "utils.h"


// Function to create a new table with a given initial capacity
table* create_table(size_t initial_capacity) {
    table *tab = (table*)malloc_with_check(sizeof(table));

    tab->entries = (table_entry*)malloc_with_check(initial_capacity * sizeof(table_entry));

    tab->size = 0;
    tab->capacity = initial_capacity;
    return tab;
}

// Function to add an entry to the table
void add_table_item(table *tab, const char *key, long value, symbol_type type) {
    // Resize the table if necessary
    if (tab->size == tab->capacity) {
        tab->capacity *= 2;
        tab->entries = (table_entry*)realloc_with_check(tab->entries, tab->capacity * sizeof(table_entry));
    }

    // Add the new entry to the table
    tab->entries[tab->size].key = strdup(key); // Allocate and copy the key string
    if (tab->entries[tab->size].key == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    tab->entries[tab->size].value = value;
    tab->entries[tab->size].type = type;
    tab->size++;
}

// Function to find an entry in the table by key

table_entry *find_by_types(table *tab, const char *key, int symbol_count, ...) {
    int i, j;
    if (tab == NULL || key == NULL || symbol_count <= 0) {
        return NULL; // Return NULL if table, key is NULL, or symbol_count is invalid
    }

    va_list arglist;
    symbol_type *valid_symbol_types = malloc(symbol_count * sizeof(symbol_type));
    if (valid_symbol_types == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }

    // Initialize the variable argument list
    va_start(arglist, symbol_count);
    for (i = 0; i < symbol_count; i++) {
        valid_symbol_types[i] = va_arg(arglist, symbol_type);
    }
    va_end(arglist);

    // Iterate over the table entries
    for (i = 0; i < tab->size; i++) {
        for (j = 0; j < symbol_count; j++) {
            if (tab->entries[i].key != NULL && strcmp(tab->entries[i].key, key) == 0 &&
                tab->entries[i].type == valid_symbol_types[j]) {
                free(valid_symbol_types); // Free allocated memory
                return &tab->entries[i];  // Return the matching entry
            }
        }
    }

    free(valid_symbol_types); // Free allocated memory
    return NULL; // Return NULL if no matching entry is found
}

// Function to free all memory associated with the table
void free_table(table *tab) {
    for (int i = 0; i < tab->size; i++) {
        free(tab->entries[i].key); // Free each key string
    }
    free(tab->entries); // Free the array of entries
    free(tab); // Free the table structure itself
}


void add_value_to_symbol_type(table *tab, long add_value, symbol_type type) {
    int i;
    if (tab == NULL) return;

    for (i = 0; i < tab->size; i++) {
        if (tab->entries[i].type == type) {
            tab->entries[i].value += add_value;
        }
    }
}

table *filter_table_by_type(table *tab, symbol_type type) {
    int i;

    if (tab == NULL) {
        return NULL; // Return NULL if table is NULL
    }

    table *filtered_table = create_table(tab->capacity); // Create a new table
    if (filtered_table == NULL) {
        return NULL; // Return NULL if table creation fails
    }

    // Iterate over the table entries
    for (i = 0; i < tab->size; i++) {
        if (tab->entries[i].type == type) {
            add_table_item(filtered_table, tab->entries[i].key, tab->entries[i].value, tab->entries[i].type);
        }
    }

    return filtered_table;
}