#include "frontier.h"

#ifndef LINKED_FRONTIERS_H
#define LINKED_FRONTIERS_H

typedef struct flink {
    frontier* lfr;
    struct flink* next;
} flink;

typedef struct linked_frnts {
    long tot_size;
    flink* head;
    flink* tail;
} linked_frnts;

linked_frnts* make_lfrnts();
void add_frnt(linked_frnts* lfrnt, frontier* frnt);
void free_lfrnts(linked_frnts* lfrnt);

#endif
