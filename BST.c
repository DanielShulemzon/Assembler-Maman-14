#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BST.h"
#include "globals.h"
#include "utils.h"

/* Function to create a new node */
Node* create_node(const char *name) {
    Node *node = malloc_with_check(sizeof(Node));
    if (name) {
        node->name = malloc_with_check(strlen(name) + 1);
        strcpy(node->name, name);
    } else {
        node->name = NULL;
    }
    node->lines = NULL;
    node->line_count = 0;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/* Function to free a node */
void free_node(Node *node) {
    int i;
    if (node) {
        free(node->name);
        for (i = 0; i < node->line_count; i++) {
            free(node->lines[i]);
        }
        free(node->lines);
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

/* Function to create a binary search tree */
BST* create_bst() {
    BST *bst = malloc_with_check(sizeof(BST));
    bst->root = NULL;
    return bst;
}

/* Function to insert a node into the BST */
Node* bst_insert(BST *bst, const char *name) {
    Node **current = &(bst->root);

    while (*current != NULL) {
        int cmp = strcmp(name, (*current)->name);
        if (cmp < 0) {
            current = &((*current)->left);
        } else if (cmp > 0) {
            current = &((*current)->right);
        } else {
            /* If the name already exists, return null */
            return NULL;
        }
    }
    *current = create_node(name);
    return *current;
}

/* Function to search for a node in the BST */
Node* bst_search(BST *bst, const char *name) {
    Node *current = bst->root;
    
    if(name == NULL) return NULL;

    while (current != NULL) {
        int cmp = strcmp(name, current->name);
        if (cmp < 0) {
            current = current->left;
        } else if (cmp > 0) {
            current = current->right;
        } else {
            return current; /* Found */
        }
    }
    return NULL; /* Not found */
}

/* Function to add a line to a node */
void add_line(Node *node, const char *line) {
    node->lines = realloc_with_check(node->lines, (node->line_count + 1) * sizeof(char *));
    if (line) {
        node->lines[node->line_count] = malloc_with_check(strlen(line) + 1);
        strcpy(node->lines[node->line_count], line);
    } else {
        node->lines[node->line_count] = NULL;
    }
    node->line_count++;
}

/* Function to free the BST */
void free_bst(BST *bst) {
    free_node(bst->root);
    free(bst);
}