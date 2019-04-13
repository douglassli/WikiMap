#ifndef FR_PAIR_H
#define FR_PAIR_H

typedef struct fr_pair {
    long node_val;
    long dist;
} fr_pair;

fr_pair* new_pair(long nv, long d);

#endif
