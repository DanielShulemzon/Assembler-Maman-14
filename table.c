#include <stdio.h>
#include <string.h>
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
table_entry* find_table_item(table *tab, const char *key) {
    for (size_t i = 0; i < tab->size; i++) {
        if (strcmp(tab->entries[i].key, key) == 0) {
            return &tab->entries[i];
        }
    }
    return NULL; // Return NULL if the key is not found
}

// Function to free all memory associated with the table
void free_table(table *tab) {
    for (size_t i = 0; i < tab->size; i++) {
        free(tab->entries[i].key); // Free each key string
    }
    free(tab->entries); // Free the array of entries
    free(tab); // Free the table structure itself
}


bool key_exists_in_table(table *tab, const char *key) {
    if (tab == NULL || key == NULL) {
        return false; // Return false if table or key is NULL
    }

    for (size_t i = 0; i < tab->size; ++i) {
        if (tab->entries[i].key != NULL && strcmp(tab->entries[i].key, key) == 0) {
            return true; // Return true if key is found
        }
    }

    return false; // Return false if key is not found
}