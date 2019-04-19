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

typedef struct job2 {
    int thread_num;
    frontier* frnt;
    long fr_start;
    long fr_end;
    int num_threads;
} job2;

void succs_to_fr2(frontier* fr, fr_pair* start_node) {
    node* succ_nodes = map_get_node(global_map, start_node->node_val);
    for (long n = 0; n < succ_nodes->size; n++) {
        long succ = succ_nodes->data[n];
        if (global_dists->data[succ] == -1) {
            fr_pair* new_frp = new_pair(succ, start_node->dist + 1);
            push_frontier(fr, (long)new_frp);
        }
   }
}

frontier* bfs_worker2(int t_num, long fr_start, long fr_end, frontier* frnt, int num_threads) {
    frontier* out_fr = make_frontier();
    long num_expanded = 0;
    for (long i = fr_start; i < fr_end; i++) {
        fr_pair* cur_pair = (fr_pair*)frnt->vals[i];
        num_expanded++;

        if (global_dists->data[cur_pair->node_val] != -1) {
            free(cur_pair);
            continue;
        }
 
        global_dists->data[cur_pair->node_val] = cur_pair->dist;

        succs_to_fr(out_fr, cur_pair);
        free(cur_pair);
    }

    return out_fr;
}

void* bfs_worker_start2(void* arg) {
    job2 j = *((job2*)arg);
    free(arg);
    frontier* ret_fr = bfs_worker2(j.thread_num, j.fr_start, j.fr_end, j.frnt, j.num_threads);
    return (void*)ret_fr;
}

int par_bfs2(map_vec* map, long source, int num_threads, int print_output) {
    
    
    return 0;
}
