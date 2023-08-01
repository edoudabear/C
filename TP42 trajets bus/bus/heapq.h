#include <stdbool.h>

struct elem {
    int id;
    int prev;
    int priority;
};

typedef struct elem Elem;

struct heapq {
    int size;
    int capacity;
    Elem* data;
};

typedef struct heapq Heapq;

Heapq new_heapq(void);
void push(Heapq* ptr_hq, int val, int prev, int priority);
bool is_empty(Heapq hq);
Elem pop(Heapq* ptr_hq);
