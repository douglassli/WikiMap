#include <stdlib.h>
#include <stdio.h>

#include "bfs.h"
#include "map_vec.h"
#include "node.h"
#include "frontier.h"
#include "explored_vec.h"
#include "parse_file.h"


int bfs(map_vec* map, long start, long goal) {
    long nodes_expanded = 0;
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
        nodes_expanded++;
        node* curr_node = (node*)pop_first_frontier(fr);
        //printf("CUR_NODE: %ld\n", curr_node->node);
        //printf("FRONTIER SIZE: %ld, CAP: %ld, OFFSET: %ld\n", fr->size, fr->cap, fr->offset);

        if (contains_explored(ex, curr_node->node)) {
            free_node(curr_node);
            continue;
        }

        push_explored(ex, curr_node->node);

        if (curr_node->node == goal) {
            printf("PATH FOUND: ");
            print_edges(curr_node);
            printf("PATH LENGTH: %ld\n", curr_node->size - 1);
            printf("NUM NODES EXPANDED: %ld\n", nodes_expanded);
            break;
        }

        node* succ_nodes = map_get_node(map, curr_node->node);

        for (long ii  = 0; ii < succ_nodes->size; ++ii) {
            long succ = succ_nodes->data[ii];
            if (!contains_explored(ex, succ)) {
                node* temp_node = copy_node(curr_node);
                temp_node->node = succ;
                push_node(temp_node, succ);
                push_frontier(fr, (long)temp_node);
                //printf("FRONTIER SIZE: %ld, CAP: %ld, OFFSET: %ld\n", fr->size, fr->cap, fr->offset);
            }
        }
        free_node(curr_node);
    }

    return 0;
}

