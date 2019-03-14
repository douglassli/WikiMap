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
