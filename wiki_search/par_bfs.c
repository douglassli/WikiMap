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
            free_frontier(out_fr);
            while(1) {
                pthread_barrier_wait(&barrier);
                if (num_threads_finished == num_threads) {
                    return;
                }
            }
        }

        fr_pair* cur_pair = (fr_pair*)pop_first_frontier(out_fr);

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
    //unreachable
    return;
}

void* bfs_worker_start(void* arg) {
    job j = *((job*) arg);
    free(arg);
    bfs_worker(j.thread_num, j.fr_start, j.fr_end, j.frnt, j.num_threads);
    return 0;
}

void run_bfs_workers(int num_threads, frontier* start_frnt) {
    pthread_t threads[num_threads];
    long partition_size = start_frnt->size / num_threads;

    for (int i = 0; i < num_threads; i++) {
        long start = i * partition_size;
        long end;
        if (i == num_threads) {
            end = start_frnt->size;
        } else {
            end = (i + 1) * partition_size;
        }
        job* temp_job = malloc(sizeof(job));
        temp_job->thread_num = i;
        temp_job->frnt = start_frnt;
        temp_job->fr_start = start;
        temp_job->fr_end = end;
        temp_job->num_threads = num_threads;

        //printf("\nStarting thread %d\n", i);
        //printf("Frontier start: %ld\n", start);
        //printf("Frontier end:   %ld\n", end);

        pthread_t thread;
        int rv = pthread_create(&thread, 0, bfs_worker_start, temp_job);
        assert(rv == 0);
        threads[i] = thread;
    }

    for (int i = 0; i < num_threads; i++) {
        int rv = pthread_join(threads[i], 0);
        assert(rv == 0);
    }
}

int par_bfs(map_vec* map, long source, int num_threads, int print_output) {
    pthread_barrier_init(&barrier, NULL, num_threads);
    global_map = map;
    frontier* init_fr = make_frontier();
    global_dists = make_explored();
    for (long i = 0; i < global_map->size; i++) {
        push_explored(global_dists, -1);
    }
    fr_pair* init_pair = new_pair(source, 0);
    global_dists->data[source] = 0;
    //push_frontier(init_fr, (long)init_pair);
    succs_to_fr(init_fr, init_pair);
    
    run_bfs_workers(num_threads, init_fr);
    
    if (print_output) {
        for (long i = 0; i < global_dists->size; i++) {
            printf("DIST FROM %ld TO %ld = %ld\n", source, i, global_dists->data[i]);
        }
    }
    free_frontier(init_fr);
    free(init_pair);
    pthread_barrier_destroy(&barrier);

    return 0;
}
