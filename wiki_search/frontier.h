#ifndef FRONTIER_H
#define FRONTIER_H

typedef struct frontier {
    long size;
    long cap;
    long offset;
    long* vals;
} frontier;

frontier* make_frontier();
long pop_last_frontier(frontier* fr);
long pop_first_frontier(frontier* fr);
void push_frontier(frontier* fr, long val);
void free_frontier(frontier* fr);
frontier* merge_frontiers(frontier* fr1, frontier* fr2);
frontier* merge_frontier_array(frontier* fra, int num_frs);

#endif
