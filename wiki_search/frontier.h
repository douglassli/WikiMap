#ifndef FRONTIER_H
#define FRONTIER_H

typedef struct frontier {
    long size;
    long cap;
    long* vals;
} frontier;

frontier* make_frontier();

#endif
