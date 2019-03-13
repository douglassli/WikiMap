#ifndef FRONTIER_H
#define FRONTIER_H

typedef struct frontier {
    long size;
    long cap;
    long* vals;
} frontier;

frontier* make_frontier();
long pop_last_frontier(frontier* fr);
long pop_first_frontier(frontier* fr);
void push_frontier(frontier* fr, long val);
void free_frontier(frontier* fr);

#endif
