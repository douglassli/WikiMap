#ifndef EXPLORED_H
#define EXPLORED_H

typedef struct explored {
    long cap;
    long size;
    long* data;
} explored;

explored* make_explored();
void push_explored(explored* ex, long val);
void free_explored(explored* ex);
int contains_explored(explored* ex, long val);
void print_explored(explored* ex);

#endif
