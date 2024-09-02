#ifndef _BST_H
#define _BST_H

#include <stdio.h>

typedef struct Node {
    char *name;
    char **lines;
    size_t line_count;
    struct Node *left;
    struct Node *right;
} Node;

/* Define the Binary Search Tree structure */
typedef struct {
    Node *root;
} BST;

Node* create_node(const char *name);
void free_node(Node *node);

BST* create_bst();
Node* bst_insert(BST *bst, const char *name);
Node* bst_search(BST *bst, const char *name);
void free_bst(BST *bst);
void add_line(Node *node, const char *line);

#endif
