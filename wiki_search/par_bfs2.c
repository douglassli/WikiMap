#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#include "pthread_barrier.h"
#include "par_bfs.h"
#include "map_vec.h"
#include "frontier.h"
#include "explored_vec.h"
#include "node.h"
#include "fr_pair.h"

map_vec* global_map;
explored* global_dists;
pthread_barrier_t barrier;
int num_threads_finished = 0;

typedef struct job {
    int thread_num;
    frontier* frnt;
    long fr_start;
    long fr_end;
    int num_threads;
} job;

void succs_to_fr(frontier* fr, fr_pair* start_node) {
    node* succ_nodes = map_get_node(global_map, start_node->node_val);
    for (long n = 0; n < succ_nodes->size; n++) {
        long succ = succ_nodes->data[n];
        if (global_dists->data[succ] == -1) {
            fr_pair* new_frp = new_pair(succ, start_node->dist + 1);
            push_frontier(fr, (long)new_frp);
        }
   }
}

int par_bfs2(map_vec* map, long source, int num_threads, int print_output) {
    
    
    return 0;
}
