#ifndef _BST_H
#define _BST_H

#include <stdio.h>

typedef struct Node {
    char *name;
    FILE *file;
    struct Node *left;
    struct Node *right;
} Node;

// Define the Binary Search Tree structure
typedef struct {
    Node *root;
} BST;

// Function prototypes
Node* create_node(const char *name, FILE *file);
void free_node(Node *node);

BST* create_bst();
void bst_insert(BST *bst, const char *name, FILE *file);
Node* bst_search(BST *bst, const char *name);
// void inorder_traversal(Node *node);
void free_bst(BST *bst);


#endif