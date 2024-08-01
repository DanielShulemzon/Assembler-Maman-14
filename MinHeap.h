#ifndef _MIN_HEAP_H
#define _MIN_HEAP_H

typedef struct {
    char **data;
    int size;
    int capacity;
} MinHeap;

MinHeap* create_heap(int capacity);
void insert(MinHeap *heap, char *key);
char* extract_min(MinHeap *heap);
void heapify_down(MinHeap *heap, int index);
void heapify_up(MinHeap *heap, int index);
void free_heap(MinHeap *heap); 


#endif