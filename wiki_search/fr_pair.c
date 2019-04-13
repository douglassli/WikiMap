#include <stdio.h>
#include <stdlib.c>

#include "fr_pair.h"

fr_pair* new_pair(long nv, long d) {
    fr_pair* frp = malloc(sizeof(fr_pair));
    frp->node_val = nv;
    frp->dist = d;
    return frp;
}


