#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "MinHeap.h"
#include "globals.h"
#include "utils.h"

MinHeap* create_heap(int capacity) {
    MinHeap *heap = malloc_with_check(sizeof(MinHeap));
    heap->data = malloc_with_check(sizeof(char*) * capacity);
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void swap(char **a, char **b) {
    char *temp = *a;
    *a = *b;
    *b = temp;
}

void insert(MinHeap *heap, char *key) {
    if (heap->size == heap->capacity) {
        fprintf(stderr, "Heap overflow\n");
        return;
    }
    heap->data[heap->size] = strdup(key);
    heap->size++;
    heapify_up(heap, heap->size - 1);
}

void heapify_up(MinHeap *heap, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (memcmp(heap->data[index], heap->data[parent], strlen(heap->data[index])) < 0) {
            swap(&heap->data[index], &heap->data[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

char* extract_min(MinHeap *heap) {
    if (heap->size == 0) {
        return NULL;
    }
    char *min = heap->data[0];
    heap->data[0] = heap->data[heap->size - 1];
    heap->size--;
    heapify_down(heap, 0);
    return min;
}

void heapify_down(MinHeap *heap, int index) {
    while (index * 2 + 1 < heap->size) {
        int smallest = index;
        int left = index * 2 + 1;
        int right = index * 2 + 2;

        if (left < heap->size && memcmp(heap->data[left], heap->data[smallest], strlen(heap->data[left])) < 0) {
            smallest = left;
        }
        if (right < heap->size && memcmp(heap->data[right], heap->data[smallest], strlen(heap->data[right])) < 0) {
            smallest = right;
        }
        if (smallest != index) {
            swap(&heap->data[index], &heap->data[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

void free_heap(MinHeap *heap) {
    for (int i = 0; i < heap->size; i++) {
        free(heap->data[i]);
    }
    free(heap->data);
    free(heap);
}