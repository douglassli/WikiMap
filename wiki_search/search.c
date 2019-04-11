#include <stdlib.h>
#include <stdio.h>

#include "search.h"
#include "map_vec.h"
#include "node.h"
#include "frontier.h"
#include "explored_vec.h"
#include "parse_file.h"

node* get_node(map_vec* map, long node_val) {
    return &map->nodes[node_val];
}


int bfs(map_vec* map, long start, long goal) {
    node* initial_node = make_node(start);
    push_node(initial_node, start);
    frontier* fr = make_frontier();
    push_frontier(fr, (long)initial_node);
    explored* ex = make_explored();

    while (1) {
        if (fr->size <= 0) {
            printf("FRONTIER EMPTY, SEARCH FAILED\n");
            return -1;
        }
        node* curr_node = (node*)pop_first_frontier(fr);
        printf("CUR_NODE: %ld\n", curr_node->node);
        printf("FRONTIER SIZE: %ld\n", ex->size);

        if (contains_explored(ex, curr_node->node)) {
            continue;
        }

        push_explored(ex, curr_node->node);

        if (curr_node->node == goal) {
            printf("PATH FOUND: ");
            print_edges(curr_node);
            break;
        }

        if (curr_node->node >= map->size) {
            continue;
        }

        node* succ_nodes = get_node(map, curr_node->node);

        for (long ii  = 0; ii < succ_nodes->size; ++ii) {
            long succ = succ_nodes->data[ii];
            if (!contains_explored(ex, succ)) {
                node* temp_node = copy_node(curr_node);
                temp_node->node = succ;
                push_node(temp_node, succ);
                push_frontier(fr, (long)temp_node);
            }
        }
    }

    return 0;
}

int main() {
    printf("PARSING...\n");
    map_vec* map = parse_map_file("map.txt");
    printf("SEARCHING...\n");
    int rv = bfs(map, 1, 52);
    printf("BFS RV: %d\n", rv);
}
