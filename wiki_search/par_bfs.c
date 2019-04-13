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
pthread_barrier_t barrier;

typedef struct job {
    int thread_num;
    frontier* frnt;
    long fr_start;
    long fr_end;
} job;

void succs_to_fr(frontier* fr, fr_pair* start_node) {
    node* succ_nodes = map_get_node(map, start_node->node_val);
    for (long i = 0; i < succ_nodes->size; i++) {
        long succ = succ_nodes->data[i];
        if (global_dists->data[succ] == -1) {
            fr_pair* new_frp = new_pair(succ, start_node->dist + 1);
            push_frontier(fr, (long)new_frp);
        }
   }   
}

frontier* bfs_worker(int t_num, long fr_start, long fr_end, frontier* frnt) {
    
}

void* bfs_worker_start(void* arg) {
    job j = *((job*) arg);
    free(arg);
    
}


int par_bfs(map_vec* map, long source, int num_threads) {
    pthread_barrier_init(&barrier, NULL, num_threads);
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
