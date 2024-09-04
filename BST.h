/* Impliments a binary tree that contains data on macros.*/
#ifndef _BST_H
#define _BST_H

#include <stdio.h>

/* define a node in the tree*/
typedef struct Node {
    /* macro name*/
    char *name;
    /* the lines that are between 'macr' and 'endmacr'. */
    char **lines;
    /* the amount of lines found.*/
    size_t line_count;
    /* left and right nodes to go through the binary search tree. */
    struct Node *left;
    struct Node *right;
} Node;

/* Define the Binary Search Tree structure */
typedef struct {
    /* the head of the binary search tree. */
    Node *root;
} BST;


/*
  Creates a new node to store a macro name.
  @param name - The macro name.
  @return - Pointer to the created node.
*/
Node* create_node(const char *name);

/*
  Frees memory for a node and its data.
  @param node - Pointer to the node to free.
*/
void free_node(Node *node);

/*
  Creates an empty binary search tree.
  @return - Pointer to the created BST.
*/
BST* create_bst();

/*
  Inserts a node with a macro name into the BST.
  @param bst - Pointer to the binary search tree.
  @param name - The macro name to insert.
  @return - Pointer to the inserted or found node.
*/
Node* bst_insert(BST *bst, const char *name);

/*
  Searches for a macro name in the BST.
  @param bst - Pointer to the binary search tree.
  @param name - The macro name to search for.
  @return - Pointer to the found node or NULL.
*/
Node* bst_search(BST *bst, const char *name);

/*
  Frees the memory for the entire BST.
  @param bst - Pointer to the binary search tree to free.
*/
void free_bst(BST *bst);

/*
  Adds a line of code to a node's lines array.
  @param node - Pointer to the node.
  @param line - Line of code to add.
*/
void add_line(Node *node, const char *line);

#endif
