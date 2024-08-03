#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BST.h"
#include "globals.h"
#include "utils.h"


Node* create_node(const char *name, FILE *file) {
    Node *node = malloc_with_check(sizeof(Node));
    node->name = strdup(name);
    node->file = file;
    node->left = NULL;
    node->right = NULL;
    return node;
}

void free_node(Node *node) {
    if (node) {
        free(node->name);
        if (node->file) {
            fclose(node->file);
        }
        free_node(node->left);
        free_node(node->right);
        free(node);
    }
}

BST* create_bst() {
    BST *bst = malloc_with_check(sizeof(BST));
    bst->root = NULL;
    return bst;
}

void bst_insert(BST *bst, const char *name, FILE *file) {
    Node **current = &(bst->root);
    while (*current != NULL) {
        int cmp = strcmp(name, (*current)->name);
        if (cmp < 0) {
            current = &((*current)->left);
        } else if (cmp > 0) {
            current = &((*current)->right);
        } else {
            // If the name already exists, update the file pointer
            if ((*current)->file) {
                fclose((*current)->file);
            }
            (*current)->file = file;
            return;
        }
    }
    *current = create_node(name, file);
}

Node* bst_search(BST *bst, const char *name) {
    Node *current = bst->root;
    while (current != NULL) {
        int cmp = strcmp(name, current->name);
        if (cmp < 0) {
            current = current->left;
        } else if (cmp > 0) {
            current = current->right;
        } else {
            return current; // Found
        }
    }
    return NULL; // Not found
}

// void inorder_traversal(Node *node) {
//     if (node) {
//         inorder_traversal(node->left);
//         printf("Name: %s\n", node->name);
//         if (node->file) {
//             printf("File pointer: %p\n", (void *)node->file);
//         }
//         inorder_traversal(node->right);
//     }
// }

void free_bst(BST *bst) {
    free_node(bst->root);
    free(bst);
}
