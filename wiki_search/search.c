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
    push_node(initial_node, start);
    frontier* fr = make_frontier();
    push_frontier(fr, (long*)initial_node);
    explored* ex = make_explored();

    while (1) {
        if (fr->size <= 0) {
            return -1;
        }

        node* curr_node = (node*)pop_first_frontier(fr);

        if (contains_explored(ex, curr_node->node)) {
            continue;
        }

        explored_push(ex, curr_node->node);

        if (curr_node->node == goal) {
            printf("PATH FOUND: \n");
            print_node(curr_node);
            break;
        }

        node* succ_nodes = get_node(map, curr_node->node);

        for (long ii  = 0; ii < succ_nodes->size; ++ii) {
            long succ = succ_nodes->data[ii];
            if (!contains_explored(ex, succ)) {
                node* temp_node = copy_node(curr_node);
                temp_node->node = succ;
                push_node(temp_node, succ);
                push_frontier(fr, (long*)temp_node);
            }
        }
    }

    return 0;
}

