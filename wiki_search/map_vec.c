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
    map->size = 0;
    map->cap = 2;
    return map;
}

void free_map(map_vec* map) {
    for (long i = 0; i < map->size; ++i) {
        free_node(&map->nodes[i]);
    }
    free(map->nodes);
    free(map);
}

void push_map(map_vec* map, node* nd) {
    if (map->size >= map->cap) {
        map->cap *= 2;
        map->nodes = realloc(map->nodes, map->cap * sizeof(node));
    }

    map->nodes[map->size] = *nd;
    map->size += 1;
}

void print_map(map_vec* map) {
    for (long i = 0; i < map->size; ++i) {
        print_node(&map->nodes[i]);
    }
}
