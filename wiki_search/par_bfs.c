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
int num_threads_finished = 0;

typedef struct job {
    int thread_num;
    frontier* frnt;
    long fr_start;
    long fr_end;
    int num_threads;
} job;

void succs_to_fr(frontier* fr, fr_pair* start_node) {
    node* succ_nodes = map_get_node(map, start_node->node_val);
    for (long n = 0; n < succ_nodes->size; n++) {
        long succ = succ_nodes->data[n];
        if (global_dists->data[succ] == -1) {
            fr_pair* new_frp = new_pair(succ, start_node->dist + 1);
            push_frontier(fr, (long)new_frp);
        }
   }   
}

void bfs_worker(int t_num, long fr_start, long fr_end, frontier* frnt, int num_threads) {
    frontier* out_fr = make_frontier();

    for (long i = fr_start; i < fr_end; i++) {
        fr_pair* cur_pair = (fr_pair*)frnt->vals[i];

        if (global_dists->data[cur_pair->node_val] != -1) {
            free(cur_pair);
            continue;
        }
        
        global_dists->data[cur_pair->node_val] = cur_pair->dist;

        succs_to_fr(out_fr, cur_pair);
        free(cur_pair);
    }

    int cur_layer = 1;
    while(1) {
        if (out_fr-> size <= 0) {
            num_threads_finished++;
            while(1) {
                pthread_barrier_wait(&barrier);
                if (num_threads_finished == num_threads) {
                    return 0;
                }
            }
        }

        fr_pair* cur_pair = (fr_pair*)frontier_pop_first(out_fr);

        if (cur_pair->dist > cur_layer) {
            pthread_barrier_wait(&barrier);
            cur_layer++;
        }

        if (global_dists->data[cur_pair->node_val] != -1) {
            free(cur_pair);
            continue;
        }
        global_dists->data[cur_pair->node_val] = cur_pair->dist;
        succs_to_fr(out_fr, cur_pair);
        free(cur_pair);
    }

    return 0;
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
