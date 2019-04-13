#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "par_bfs.h"
#include "map_vec.h"
#include "frontier.h"
#include "explored_vec.h"
#include "node.h"

map_vec* global_map;
frontier* global_fr;
explored* global_dists;

int par_bfs(map_vec* map, long source) {
    global_map = map;
    global_fr = make_frontier();
    global_dists = make_explored();
    for (long i = 0; i < global_map->size; i++) {
        push_explored(global_dists, -1);
    }

    return 0;
}
