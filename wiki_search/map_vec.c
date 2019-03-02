#include <stdio.h>
#include <stdlib.h>

#include "map_vec.h"
#include "node.h"

/*
	typedef struct map_vec {
		node* nodes;
		long size;
		long cap;
	} map_vec;
*/

map_vec* make_map() {
	map_vec* map = malloc(sizeof(map_vec));
	map->nodes = malloc(2 * sizeof(node));
	xs->size = 0;
	xs->cap = 2;
	return map;
}

void free_map(map_vec* map) {
	for (int i = 0; i < map->size; ++i) {
		free_node(map->nodes[i]);
	}
	free(map->nodes);
	free(map)
}
