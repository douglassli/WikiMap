#include node.h

#ifndef MAP_VEC_H
#define MAP_VEC_H

typedef struct map_vec {
	node* nodes;
	long size;
	long cap;
} map_vec;

map_vec* make_map();
void free_map(map_vec* map);
void push_map(map_vec* map, node* node);

#endif
