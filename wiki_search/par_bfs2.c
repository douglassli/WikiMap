#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

#include "par_bfs.h"
#include "map_vec.h"
#include "frontier.h"
#include "explored_vec.h"
#include "node.h"
#include "fr_pair.h"

map_vec* global_map2;
explored* global_dists2;

typedef struct job2 {
    int thread_num;
    frontier* frnt;
    long fr_start;
    long fr_end;
    int num_threads;
} job2;

void succs_to_fr2(frontier* fr, fr_pair* start_node) {
    node* succ_nodes = map_get_node(global_map2, start_node->node_val);
    for (long n = 0; n < succ_nodes->size; n++) {
        long succ = succ_nodes->data[n];
        if (global_dists2->data[succ] == -1) {
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

        if (global_dists2->data[cur_pair->node_val] != -1) {
            free(cur_pair);
            continue;
        }
 
        global_dists2->data[cur_pair->node_val] = cur_pair->dist;

        succs_to_fr2(out_fr, cur_pair);
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

void run_bfs_workers2(int num_threads, frontier* start_fr) {
    frontier* cur_fr = start_fr;
    while(1) {
        if (cur_fr->size <= 0) {
            return;
        }

        pthread_t threads[num_threads];
        long partition_size = cur_fr->size / num_threads;

        for (int i = 0; i < num_threads; i++) {
            long start = i * partition_size;
            long end;
            if (i == num_threads) {
                end = start_fr->size;
            } else {
                end = (i + 1) * partition_size;
            }
            job2* temp_job = malloc(sizeof(job2));
            temp_job->thread_num = i;
            temp_job->frnt = cur_fr;
            temp_job->fr_start = start;
            temp_job->fr_end = end;
            temp_job->num_threads = num_threads;

            //printf("\nStarting thread %d\n", i);
            //printf("Frontier start: %ld\n", start);
            //printf("Frontier end:   %ld\n", end);

            pthread_t thread;
            int rv = pthread_create(&thread, 0, bfs_worker_start2, temp_job);
            assert(rv == 0);
            threads[i] = thread;
        }
        
        //frontier* new_fr = make_frontier();
        frontier* fra[num_threads];
        for (int i = 0; i < num_threads; i++) {
            void* ret_val;
            int rv = pthread_join(threads[i], &ret_val);
            assert(rv == 0);
            frontier* thread_frnt = (frontier*)ret_val;
            fra[i] = thread_frnt;
            //new_fr = merge_frontiers(thread_frnt, new_fr);
        }
        //free_frontier(cur_fr);
        cur_fr = merge_frontier_array((frontier*)fra, num_threads);//new_fr;
    }    
}

int par_bfs2(map_vec* map, long source, int num_threads, int print_output) {
    global_map2 = map;
    frontier* init_fr = make_frontier();
    global_dists2 = make_explored();
    for (long i = 0; i < global_map2->size; i++) {
        push_explored(global_dists2, -1);
    }
    fr_pair* init_pair = new_pair(source, 0);
    global_dists2->data[source] = 0;
    succs_to_fr2(init_fr, init_pair);

    run_bfs_workers2(num_threads, init_fr);
    
    if (print_output) {
        for (long i = 0; i < global_dists2->size; i++) {
            printf("DIST FROM %ld TO %ld = %ld\n", source, i, global_dists2->data[i]);
        }
    }
    free(init_pair);
    free_explored(global_dists2);

    return 0;
}
