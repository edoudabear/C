#include <stdio.h>
#include <stdlib.h>
#include "heapq.h"

Heapq new_heapq(void) {
    Heapq hq = { .capacity = 0, .size = 0, .data=(Elem*)(malloc(100000*sizeof(Elem))) };
    return hq;
}

void push(Heapq* ptr_hq, int id, int prev, int priority) {
    Elem e = { .id = id, .prev = prev, .priority = priority };
    ptr_hq->data[ptr_hq->size] = e;
    ptr_hq->size++;
}

bool is_empty(Heapq hq) {
    return hq.size == 0;
}

Elem pop(Heapq* ptr_hq) {
    if (ptr_hq->size==0) { printf("Empty queue\n"); exit(-1); }
    int best = 0;
    for (int i=1; i<ptr_hq->size; ++i) {
        if (ptr_hq->data[i].priority < ptr_hq->data[best].priority) { best=i; }
    }
    ptr_hq->size--;
    Elem res = ptr_hq->data[best];
    ptr_hq->data[best] = ptr_hq->data[ptr_hq->size];
    return res;
}

