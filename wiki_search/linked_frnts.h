#include "frontier.h"

#ifndef LINKED_FRONTIERS_H
#define LINKED_FRONTIERS_H

typedef struct flink {
    frontier* frnt;
    struct alink array* next;
} flink;

typedef struct linked_frnts {
    long tot_size;
    flink* head;
    flink* tail;
}

#endif
