#include <stdio.h>
#include <stdlib.h>

#include "frontier.h"
#include "linked_frnts.h"

linked_frnts* make_lfrnts() {
    linked_frnts* new_lfrnt = malloc(sizeof(linked_frnts));
    new_lfrnt->tot_size = 0;
    new_lfrnt->head = 0;
    new_lfrnt->tail = 0;
    return new_lfrnt;    
}

void add_frnt(linked_frnts* lfrnt, frontier* frnt) {
    flink* new_flink = malloc(sizeof(flink));
    new_flink->lfr = frnt;
    new_flink->next = 0;

    lfrnt->tail->next = new_flink;
    lfrnt->tail = new_flink;
    lfrnt->size += frnt->size;
}

void free_lfrnts(linked_frnts* lfrnt) {
    flink* cur_link = lfrnt->head;
    while(1) {
        if ((long)cur_link->next == 0) {
            break;
        }
        flink* nlink = cur_link->next;
        free_frontier(cur_link->lfr);
        free(cur_link);
        cur_link = nlink;
    }
    free(lfrnt);
}
