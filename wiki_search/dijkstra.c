#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"
#include "map_vec.h"
#include "node.h"
#include "frontier.h"
#include "explored_vec.h"

node* get_node(map_vec* map, long node_val) {
    return &map->nodes[node_val];
}

int dijkstra(map_vec* map, long source) {
    explored* layers = make_explored();
    for (long i = 0; i < map->size; i++) {
        push_explored(layers, -1);
    }
    layers->data[source] = 0;
}
