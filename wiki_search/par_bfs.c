#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "par_bfs.h"
#include "map_vec.h"
#include "frontier.h"
#include "explored_vec.h"
#include "node.h"
#include "fr_pair.h"

map_vec* global_map;
frontier* global_fr;
explored* global_dists;

void succs_to_fr(frontier* fr, fr_pair* start_node) {
    
}

int par_bfs(map_vec* map, long source) {
    global_map = map;
    global_fr = make_frontier();
    global_dists = make_explored();
    for (long i = 0; i < global_map->size; i++) {
        push_explored(global_dists, -1);
    }
    fr_pair* init_pair = new_pair(source, 0);
    push_frontier(global_fr, (long)init_pair);



    return 0;
}
