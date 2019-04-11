#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
    fr->offset = 0;
    return fr;
}

void push_frontier(frontier* fr, long val) {
    if (fr->size + fr->offset >= fr->cap) {
        if (fr->offset > 0) {
            fr->cap = (fr->offset + fr->cap) * 2;
            void* new_data = malloc(fr->cap);
            memcpy(new_data, fr->vals + (fr->offset * sizeof(long)), fr->size * sizeof(long));
            free(fr->vals);
            fr->vals = (long*)new_data;
            fr->offset = 0;
        } else {
            fr->cap *= 2;
            fr->vals = realloc(fr->vals, fr->cap * sizeof(long));
        }
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

    long* array_ptr = fr->vals + (fr->offset * sizeof(long));

    long return_val = array_ptr[fr->size];
    fr->size -= 1;

    if (fr->size < fr->cap / 2) {
        fr->cap /= 2;
        fr->vals = realloc(fr->vals, fr->cap * sizeof(long));
    }
    
    return return_val;
}

long pop_first_frontier(frontier* fr) {
    if (fr->size <= 0) {
        return -1;
    }
    long* array_ptr = fr->vals + (fr->offset * sizeof(long));

    long return_val = array_ptr[0];
    fr->size -= 1;
    fr->cap -= 1;
    fr->offset++;
    //fr->vals = realloc(fr->vals + sizeof(long), fr->cap * sizeof(long));
    //void* new_data = malloc(fr->cap * sizeof(long));
    //memcpy(new_data, (void*)fr->vals + sizeof(long), fr->cap * sizeof(long));
    //free(fr->vals);
    //fr->vals = (long*)new_data;
    return return_val;
}
