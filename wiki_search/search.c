#include <stdlib.h>
#include <stdio.h>

#include "search.h"
#include "map_vec.h"
#include "node.h"
#include "frontier.h"
#include "explored_vec.h"

node* get_node(map_vec* map, long node_val) {
    return map->nodes[node_val];
}


int bfs(map_vec* map, long start, long goal) {
    node* initial_node = make_node(start);
    frontier* fr = make_frontier();
    push_frontier(fr, (long*)initial_node);
    explored* ex = make_explored();

    while (1) {
        if (fr->size <= 0) {
            return -1;
        }

        node* curr_node = (node*)pop_first_frontier(fr);
        explored_push(ex, curr_node->node);

        if (curr_node->node == goal) {
            printf("PATH FOUND: \n");
            print_node(curr_node);
            break;
        }


    }

    return 0;
}

