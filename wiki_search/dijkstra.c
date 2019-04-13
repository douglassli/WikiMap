#include <stdio.h>
#include <stdlib.h>

#include "dijkstra.h"
#include "map_vec.h"
#include "node.h"
#include "frontier.h"
#include "explored_vec.h"
#include "fr_pair.h"

int dijkstra(map_vec* map, long source) {
    explored* layers = make_explored();
    for (long i = 0; i < map->size; i++) {
        push_explored(layers, -1);
    }
    layers->data[source] = 0;
    frontier* fr = make_frontier();
    fr_pair* init_pair = new_pair(source, 0);
    push_frontier(fr, (long)init_pair);
    long nodes_expanded = 0;

    while(1) {
        if (fr->size <= 0) {
            break;
        }

        nodes_expanded++;
        fr_pair* cur_pair = (fr_pair*)pop_first_frontier(fr);

        if (layers->data[cur_pair->node_val] != -1 && cur_pair->node_val != source) {
            free(cur_pair);
            continue;
        }

        layers->data[cur_pair->node_val] = cur_pair->dist;

        node* succ_nodes = map_get_node(map, cur_pair->node_val);
        for (long i = 0; i < succ_nodes->size; i++) {
            long succ = succ_nodes->data[i];
            if (layers->data[succ] == -1) {
                fr_pair* new_frp = new_pair(succ, cur_pair->dist + 1);
                push_frontier(fr, (long)new_frp);
            }
        }
        free(cur_pair);
    }

    for (long i = 0; i < map->size; i++) {
        printf("DIST FROM %ld TO %ld = %ld\n", source, i, layers->data[i]);
    }
    return 0;
}
