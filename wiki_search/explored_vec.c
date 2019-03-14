#include <stdlib.h>
#include <stdio.h>

#include "explored_vec.h"

explored* make_explored() {
    explored* ex = malloc(sizeof(explored));
    ex->cap = 2;
    ex->size = 0;
    ex->data = malloc(ex->cap * sizeof(long));
    return ex;
}

void push_explored(explored* ex, long val) {
    if (ex->size >= ex->cap) {
        ex->cap *= 2;
        ex->data = realloc(ex->data, ex->cap * sizeof(long));
    }

    ex->data[ex->size] = val;
    ex->size += 1;
}

void free_explored(explored* ex) {
    free(ex->data);
    free(ex);
}
