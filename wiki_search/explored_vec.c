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

int contains_explored(explored* ex, long val) {
    for (long i = 0; i < ex->size; ++i) {
        if (ex->data[i] == val) {
            return 1;
        }
    }
    return 0;
}

void print_explored(explored* ex) {
    printf("Explored Set: \n");
    for (long i = 0; i < ex->size; ++i) {
        printf("Value %ld = %ld\n", i, ex->data[i]);
    }
}
