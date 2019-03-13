#include <stdlib.c>
#include <stdio.h>

#include "frontier.h"

/*
    typedef struct frontier {
        long size;
        long cap;
        long* vals;
    } frontier;
*/

frontier* make_frontier() {
    frontier* fr = malloc(sizeof(frontier));
    fr->vals = malloc(2 * sizeof(long));
    fr->size = 0;
    fr->cap = 2;
    return fr;
}

void push_frontier(frontier* fr, long val) {
    if (fr->size >= fr->cap) {
        fr->cap *= 2;
        fr->vals = realloc(fr->vals, fr->cap * sizeof(long));
    } 

    fr->vals[fr->size] = val;
    fr->size += 1;
}

void free_frontier(frontier* fr) {
    free(fr->vals);
    free(fr);
}

long pop_last_frontier(frontier* fr) {
    if (fr->size <= 0) {
        return -1;
    }

    long return_val = fr->vals[fr->size];
    fr->size -= 1;

    if (fr->size < fr->cap / 2) {
        fr->cap /= 2;
        fr->vals = realloc(fr->vals, fr->cap * sizeof(long));
    }
    
    return return_val;
}

long pop_first_frontier(frontier* fr) {
    
}
