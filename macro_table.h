#ifndef _MACRO_TABLE_H
#define _MACRO_TABLE_H

typedef struct table_node{
    char *macro_name;
    macro_node left;
    macro_node right; 
}macro_node;

#endif