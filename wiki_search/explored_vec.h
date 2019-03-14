#ifndef EXPLORED_H
#define EXPLORED_H

typedef struct explored {
    long cap;
    long size;
    long* data;
} explored;

explored* make_explored();


#endif
